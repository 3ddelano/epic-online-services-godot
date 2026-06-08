# eos_function_usage_checker.py

import os
import re


# Function to extract function names from the EOS C SDK header files
def extract_function_names(sdk_path):
    function_declarations = {}
    pattern = re.compile(r"EOS_DECLARE_FUNC\(\w+\)\s+(EOS_\w+)\(")

    for root, _, files in os.walk(sdk_path):
        for file in files:
            if file.endswith(".h"):
                with open(os.path.join(root, file), 'r', encoding='utf-8') as f:
                    content = f.read()
                    matches = pattern.findall(content)
                    for match in matches:
                        function_declarations[match] = os.path.join(root, file)

    return function_declarations


# Function to search for function names in the src folder
def find_function_calls(src_path, function_names):
    function_usage = {name: [] for name in function_names}
    pattern = re.compile(r"(\bEOS_\w+\b)")

    for root, _, files in os.walk(src_path):
        for file in files:
            if file.endswith(".h") or file.endswith(".cpp"):
                with open(os.path.join(root, file), 'r', encoding='utf-8') as f:
                    content = f.read()
                    matches = pattern.findall(content)
                    for match in matches:
                        if match in function_usage:
                            function_usage[match].append(os.path.join(root, file))

    return function_usage


def main():
    sdk_path = 'thirdparty/eos-sdk/SDK'  # Replace with the actual path to the EOS C SDK folder
    src_path = 'src'  # Replace with the actual path to the src folder

    whitelist = ['EOS_Achievements_CopyAchievementDefinitionByIndex',
                 'EOS_Achievements_CopyAchievementDefinitionByAchievementId',
                 'EOS_Achievements_GetUnlockedAchievementCount',
                 'EOS_Achievements_CopyUnlockedAchievementByIndex',
                 'EOS_Achievements_CopyUnlockedAchievementByAchievementId',
                 'EOS_Achievements_AddNotifyAchievementsUnlocked',
                 'EOS_AntiCheatClient_PollStatus',
                 'EOS_ByteArray_ToString',
                 'EOS_EpicAccountId_IsValid',
                 'EOS_ProductUserId_IsValid',
                 ]

    # Step 1: Extract function names from SDK header files
    function_declarations = extract_function_names(sdk_path)
    function_names = list(function_declarations.keys())
    print(f"Extracted {len(function_names)} function names from SDK headers.")

    # Step 2: Search for these function names in the src folder
    function_usage = find_function_calls(src_path, function_names)

    # Print the results
    print("SDK functions not called by the plugin:")
    uncalled_count = 0
    for function, files in function_usage.items():
        if files: continue
        if function in whitelist: continue
        if "RemoveNotify" in function: continue

        uncalled_count += 1
        print(f"{function} : {function_declarations[function]}")

    print(f"\nTotal number of SDK functions not called by the plugin: {uncalled_count}")


if __name__ == "__main__":
    main()
