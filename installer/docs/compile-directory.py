# -*- coding: utf-8 -*-
import os

def convert_tree_to_dot(input_file, output_file):
    with open(input_file, 'rb') as f:
        lines = f.readlines()

    dot_lines = [
        'digraph G {',
        'rankdir=TB;',  # Set rank direction to top to bottom for vertical layout
        'node [shape=box width=2, height=0.6, fontsize=17];',  # Optional: Set node shape to box for better alignment
        'edge [splines=ortho];',  # Use orthogonal edges for better straight lines
        #'nodesep=1', # horizontal spacing
        'ranksep=1', # vertical spacing
    ]
    stack = []

    for line in lines:
        line = line.rstrip()

        # Skip lines containing summary information or tree structure
        if " directories" in line or line in ('', '│', '└──', '├──'):
            continue

        # Determine the depth of the current directory
        depth = line.count('│') + line.count('└──') + line.count('├──')

        # Extract the directory or file name (ignoring the tree structure characters)
        node_name = line.split('─')[-1].strip()

        # Construct the full path for the current node
        if depth == 0:
            full_path = node_name
            stack = [full_path]
            dot_lines.append('"{}" [label="{}"]'.format(full_path, node_name))
        else:
            while len(stack) > depth:
                stack.pop()

            parent_path = stack[-1]
            full_path = os.path.join(parent_path, node_name)
            stack.append(full_path)
            dot_lines.append('"{}" -> "{}"'.format(parent_path, full_path))
            dot_lines.append('"{}" [label="{}"]'.format(full_path, node_name))

    dot_lines.append('}')

    with open(output_file, 'w') as f:
        f.write('\n'.join(dot_lines))

if __name__ == "__main__":
    convert_tree_to_dot('directory-tree.txt', 'directory-tree.dot')