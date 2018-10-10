# shingine scene exporter for houdini
# *.ssd (shingine scene description)
# collect all of the object nodes in the obj

from enum import Enum

class ContainerType(Enum):
    NODE = 1
    VALUE = 3

class DataType(Enum):
    BYTE = 1
    UINT = 2
    INT = 3
    FLOAT = 4

class Node:
    def __init__(self, id, name):
        self.id = 0
        self.parent_id = -1
        self.name = name
        self.attributes = []

class Attribute:
    def __init__(self):
        self.name = "unknown_attribute"
        self.container_type = ContainerType.VALUE
        self.data_type = DataType.FLOAT
        self.element_count = 1
        self.value = None

node_name_to_id = {}
hom_nodes = []
nodes = []

# get all hom nodes
for id, node in enumerate(hou.node("/obj").children()):
    if node.type().name() not in ["light", "null", "geo"]:
        continue
    node_name_to_id[node.name()] = id
    nodes.append(Node(id, name))
    hom_nodes.append(node)

# set parent ids
for id, hom_node in hom_nodes:
    parent_id = -1
    if hom_node.inputs():
        input_node = hom_node.inputs()[0]
        if input_node.name() not in node_name_to_id.keys():
            continue
        parent_id = node_name_to_id[input_node.name()]
    nodes[id].parent_id = parent_id

# gather matrix attributes
for id, node in nodes:
    local_transform = hom_nodes[id].localTransform()

    attr = Attribute()
    attr.container_type = ContainerType.VALUE
    attr.element_count = 16
    attr.data_type = DataType.FLOAT
    attr.value = local_transform.asTuple()

    nodes[id].attributes.append(attr)
