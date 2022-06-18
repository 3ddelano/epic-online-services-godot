# Horizontal Flow Container
For use in the Godot game engine.

![Example Screenshot](screenshots/example_1.png)

Places it's children in a horizontal row according to their minimal size, until it can't fit the next child on the row. Then it starts a new row.

The node is called "HFlowContainer" and is a child of "Container".

Features:
- Updates when it's width is changed.
- Works in the editor.
- Works in ScrollContainer.
- Hiding children works.
- Customizable horizontal and vertical margins between the child nodes.
- Icon:
![HFLowContainer Icon](h_flow_container/h_flow_container.svg)

The container does (currently) not support:
- Child scaling.
- Child rotating.
- Child Size Flags ("Fill", "Expand" etc.)

# Changelog

- 1.0.0: Initial release.
- 1.0.1: Fixed "narrowing conversion" warnings.
- 1.0.2: Works by default in ScrollContainer.
         (By always settting horizontal size flag to EXPAND + FILL)
