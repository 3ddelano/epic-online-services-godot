#!/usr/bin/env python3
"""
Sort methods alphabetically in .h and .cpp files under src/.

Usage:
    python scripts/sort_methods.py                    # dry-run
    python scripts/sort_methods.py --apply            # apply changes
    python scripts/sort_methods.py --ieos-only        # ieos.h + ieos.cpp only
"""

import os
import re
import sys
import shutil
import difflib

SRC_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'src')


def is_method_declaration(line):
    """True if line is a class-scope method declaration (not inside a body)."""
    s = line.strip()
    if not s.endswith(';'):
        return False
    if not re.search(r'\([^)]*\)', s):
        return False
    # Exclude preprocessor, control flow, etc.
    if s.startswith('#') or s.startswith('//') or s.startswith('/*') or s.startswith('*'):
        return False
    if s.startswith('typedef') or s.startswith('using ') or s.startswith('template'):
        return False
    if s.startswith('for(') or s.startswith('if(') or s.startswith('while(') or s.startswith('switch('):
        return False
    # Exclude member variable init: Type name = ...;
    if re.match(r'^\s*\w[\w<>_:*&]*\s+\*?\s*\w[\w_]*\s*[=;]', s) and not re.match(r'^\s*(virtual|static|_FORCE_INLINE_)', s):
        # Has return-type-like start but no opening paren before = or ;
        if '=' in s and not re.search(r'\(\s*\)', s):
            return False
        if not re.search(r'\)\s*(const\s*)?(override\s*)?(=\s*0\s*)?;', s):
            pass  # could still be a method
    # Must have a return type or be ctor/dtor or be void
    return True


def extract_method_name(line):
    """Return the method name from a declaration line for sorting key."""
    m = re.match(
        r'^\s*'
        r'(?:virtual\s+)?'
        r'(?:static\s+)?'
        r'(?:_FORCE_INLINE_\s+)?'
        r'(?:.*?\s+)?'
        r'(~?)(\w[\w_]*)\s*\(',
        line.strip()
    )
    if m:
        return ('~' if m.group(1) else '') + m.group(2)
    # Fallback: grab first word before (
    m2 = re.search(r'(\w[\w_]*)\s*\(', line)
    return m2.group(1) if m2 else ''


def parse_class_body(lines):
    """Find class body boundaries and return (body_start, body_end) or None."""
    # Find class definition
    class_start = None
    for i, line in enumerate(lines):
        if re.match(r'^\s*(class|struct)\s+\w+', line):
            class_start = i
            break
    if class_start is None:
        return None

    # Find opening brace of class
    open_brace = None
    for i in range(class_start, len(lines)):
        if '{' in lines[i]:
            open_brace = i
            break
    if open_brace is None:
        return None

    # Find matching closing brace using depth counting
    depth = 0
    for i in range(open_brace, len(lines)):
        line = lines[i]
        # Exclude braces inside strings/comments for accuracy, but we keep it simple
        opens = line.count('{')
        closes = line.count('}')
        depth += opens - closes
        if depth <= 0:
            return (open_brace, i)

    return (open_brace, len(lines) - 1)


def is_member_var(line):
    """Check if line is a member variable declaration."""
    s = line.strip()
    if not s.endswith(';'):
        return False
    if '(' in s and ')' in s:
        return False  # has function params -> not a member var
    if re.match(r'^\s*(EOS_\w+|int|bool|String|void|Dictionary|Variant|unsigned|char|float|double|uint32_t|uint64_t|int32_t|int64_t|Ref<|EOS_NotificationId|EOS_EResult|std::)', s):
        return True
    return False


def sort_header_file(content):
    """Sort method declarations in each header file section."""
    lines = content.split('\n')

    body = parse_class_body(lines)
    if body is None:
        return content

    body_start, body_end = body

    # Collect lines in the class body, tracking block depth
    # Depth 0 = class-level scope
    depth = 0
    lines_at_depth = []  # list of (original_index, depth_at_start, is_method_decl, text)

    for i in range(body_start + 1, body_end):
        line = lines[i]
        stripped = line.strip()
        opens = line.count('{')
        closes = line.count('}')

        if depth == 0:
            # Class-level line
            is_meth = is_method_declaration(line) and not is_member_var(line) and not stripped.startswith('GDCLASS')
            lines_at_depth.append((i, depth, is_meth, line))
        else:
            lines_at_depth.append((i, depth, False, line))

        depth += opens - closes

    # Now group consecutive method declarations
    groups = []
    current_group = []
    current_indices = []

    for idx, at_depth, is_meth, text in lines_at_depth:
        if at_depth != 0:
            if current_group:
                groups.append((current_indices, current_group, False))
                current_group = []
                current_indices = []
            groups.append(([idx], [text], False))
            continue

        if is_meth:
            current_group.append(text)
            current_indices.append(idx)
        else:
            if current_group:
                groups.append((current_indices, current_group, True))
                current_group = []
                current_indices = []
            groups.append(([idx], [text], False))

    if current_group:
        groups.append((current_indices, current_group, True))

    # Sort method groups by method name
    sorted_groups = []
    for indices, texts, is_method_group in groups:
        if is_method_group:
            # Pair each text with its extracted method name and sort
            paired = [(extract_method_name(t), t) for t in texts]
            paired.sort(key=lambda x: x[0])
            sorted_texts = [p[1] for p in paired]
            sorted_groups.append((indices, sorted_texts))
        else:
            sorted_groups.append((indices, texts))

    # Rebuild, preserving original indices
    result = list(lines)
    pos = 0
    for indices, texts in sorted_groups:
        for idx, text in zip(indices, texts):
            result[idx] = text
            pos += 1

    return '\n'.join(result)


def sort_cpp_bind_methods(content):
    """Sort _bind_methods() entries alphabetically within a cpp file."""
    lines = content.split('\n')

    # Find the _bind_methods function body
    func_start = None
    open_brace = None
    close_brace = None

    for i, line in enumerate(lines):
        if '_bind_methods' in line and '{' in line:
            func_start = i
            break

    if func_start is None:
        return content

    # Locate opening brace
    for i in range(func_start, len(lines)):
        if '{' in lines[i]:
            open_brace = i
            break

    if open_brace is None:
        return content

    # Find matching closing brace
    depth = 0
    for i in range(open_brace, len(lines)):
        depth += lines[i].count('{') - lines[i].count('}')
        if depth <= 0:
            close_brace = i
            break

    if close_brace is None:
        return content

    body_lines = lines[open_brace + 1 : close_brace]

    # Categorize lines
    bind_re = re.compile(r'^\s*(?:IEOS_BIND_METHOD|ClassDB::bind_(?:static_)?method)')
    signal_re = re.compile(r'^\s*((?:IEOS_BIND_SIGNAL|ADD_SIGNAL)\()')

    categories = []  # (category, index_in_body, line)
    for idx, line in enumerate(body_lines):
        s = line.strip()
        if s == '':
            categories.append(('blank', idx, line))
        elif bind_re.match(s):
            categories.append(('bind', idx, line))
        elif signal_re.match(s):
            categories.append(('signal', idx, line))
        else:
            categories.append(('other', idx, line))

    # Group consecutive same-category
    groups = []
    current_cat = None
    current = []
    for cat, idx, line in categories:
        if cat != current_cat:
            if current:
                groups.append((current_cat, current))
            current_cat = cat
            current = []
        current.append((idx, line))
    if current:
        groups.append((current_cat, current))

    # Sort bind/signal groups by name
    def extract_bind_name(line):
        m = re.search(r'\(\s*(?:D_METHOD\()?["\']?(\w[\w_]*)', line.strip())
        if m:
            return m.group(1)
        m2 = re.search(r'IEOS_BIND_SIGNAL\s*\(\s*(\w[\w_]*)', line.strip())
        if m2:
            return m2.group(1)
        m3 = re.search(r'ADD_SIGNAL\s*\(\s*MethodInfo\s*\(\s*["\'](\w[\w_]*)', line.strip())
        if m3:
            return m3.group(1)
        return ''

    new_body = []
    for cat, group in groups:
        if cat in ('bind', 'signal'):
            group.sort(key=lambda x: extract_bind_name(x[1]))
        new_body.extend(line for _, line in group)

    # Rebuild
    result = lines[:open_brace + 1] + new_body + lines[close_brace:]
    return '\n'.join(result)


def process_header(filepath, apply_changes):
    with open(filepath, 'r') as f:
        content = f.read()
    new_content = sort_header_file(content)
    if content == new_content:
        return False
    if apply_changes:
        with open(filepath, 'w') as f:
            f.write(new_content)
        print(f"  MODIFIED: {filepath}")
    else:
        diff = difflib.unified_diff(
            content.splitlines(),
            new_content.splitlines(),
            fromfile=filepath,
            tofile=filepath + ' (sorted)',
            lineterm=''
        )
        print(f"  WOULD CHANGE: {filepath}")
        for line in diff:
            print(f"    {line}")
    return True


def process_cpp(filepath, apply_changes):
    with open(filepath, 'r') as f:
        content = f.read()
    new_content = sort_cpp_bind_methods(content)
    if content == new_content:
        return False
    if apply_changes:
        with open(filepath, 'w') as f:
            f.write(new_content)
        print(f"  MODIFIED: {filepath}")
    else:
        diff = difflib.unified_diff(
            content.splitlines(),
            new_content.splitlines(),
            fromfile=filepath,
            tofile=filepath + ' (sorted)',
            lineterm=''
        )
        print(f"  WOULD CHANGE: {filepath}")
        for line in diff:
            print(f"    {line}")
    return True


def main():
    apply_changes = '--apply' in sys.argv
    ieos_only = '--ieos-only' in sys.argv

    mode = "APPLYING" if apply_changes else "DRY-RUN (use --apply to make changes)"
    print(f"Mode: {mode}\n")

    headers = sorted(f for f in os.listdir(SRC_DIR) if f.endswith('.h'))
    cpps = sorted(f for f in os.listdir(SRC_DIR) if f.endswith('.cpp'))

    if ieos_only:
        headers = [f for f in headers if f == 'ieos.h']
        cpps = [f for f in cpps if f == 'ieos.cpp']

    changed = 0

    if headers:
        print("=== Header files ===")
        for f in headers:
            filepath = os.path.join(SRC_DIR, f)
            if process_header(filepath, apply_changes):
                changed += 1

    if cpps:
        print("=== C++ files ===")
        for f in cpps:
            filepath = os.path.join(SRC_DIR, f)
            if process_cpp(filepath, apply_changes):
                changed += 1

    if changed == 0:
        print("No files changed.")
    else:
        print(f"\n{changed} file(s) {'modified' if apply_changes else 'would be modified'}.")


if __name__ == '__main__':
    main()
