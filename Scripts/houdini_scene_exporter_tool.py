'''
HEADER 
{
    signature U24
    version U8
}
NODE
{
    node_begin U8
    unique_id U32
    name_length U8
    name U8[name_length]
    attribute_count U8
    node_count U8
    attributes ATTRIBUTE[attribute_count]
    nodes NODE[node_count]
    node_end U8
}
ATTRIBUTE
{
    attr_begin U8
    name_length U8
    name U8[name_length]
    data_type_char_length U8
    data_type U8[data_type_char_length]
    single_element U8
    byte_count U32
    element_count U32
    value U8[element_count]
    attr_end U8
}
FILE
{
    header HEADER
    node_count U16
    nodes NODE[node_count]
}
'''

import struct

DataType_NONE = 0
DataType_BYTE = 1
DataType_UINT = 2
DataType_FLOAT = 3
DataType_INT = 4
DataType_INT16 = 5
DataType_UINT16 = 6
DataType_CHAR = 7
DataType_UID = 8
DataType_SERIALIZED_CLASS = 9

DrawType_WIRE_FRAME = 0
DrawType_FILL = 1
DrawType_POINTS = 2

data_type_name_map = {
    DataType_BYTE : "unsigned char",
    DataType_INT : "int",
    DataType_INT16 : "short",
    DataType_UINT : "unsigned int",
    DataType_UINT16 : "unsigned short",
    DataType_FLOAT : "float",
    DataType_CHAR : "char",
    DataType_UID : "uid",
    DataType_SERIALIZED_CLASS : "SerializedClass"
}

NextId = 50

light_type_name = "hlight::2.0"
null_type_name = "null"
geo_type_name = "geo"

allowed_hom_types = (light_type_name, null_type_name, geo_type_name)

node_name_to_uid = {}
uid_to_hom_node = {}
uid_to_components = {}
components = []

class GeometryData:
    def __init__(self, node):
        display = node.displayNode()
        divide = node.createNode("divide")
        normal = node.createNode("normal")
        normal.parm("type").set(0)
        divide.setInput(0, display)
        normal.setInput(0, divide)
        normal.setDisplayFlag(1)
        normal.setRenderFlag(1)
        # read geometry data from triangulated node
        geometry = normal.geometry()
        positions = []
        normals = []
        indices = []
        texcoord = []
        for point in geometry.points():
            # positions
            positions.append(point.position().x())
            positions.append(point.position().y())
            positions.append(point.position().z())
            # normals as point normals
            normal_value = point.attribValue("N")
            normals.append(normal_value[0])
            normals.append(normal_value[1])
            normals.append(normal_value[2])
            # deal with uvs later
            texcoord.append(0.0)
            texcoord.append(0.0)
            texcoord.append(0.0)
        for face in geometry.prims():
            vertices = reversed(face.vertices())
            for vert in vertices:
                indices.append(vert.point().number())
        # delete temp houdini nodes
        normal.destroy()
        divide.destroy()

        self.positions = positions
        self.normals = normals
        self.indices = indices
        self.texcoord = texcoord
        

class Node:
    def __init__(self, name="Node"):
        self.name = name
        self.unique_id = GetUid()
        self.attributes = []
        self.nodes = []

class Attribute:
    def __init__(self, name, data_type, value, single_element=False):
        self.name = name
        self.data_type = data_type
        self.single_element = single_element
        self.value = value

def GetUid():
    global NextId
    uid = NextId
    NextId += 1
    return uid

def parent_node_uid(hom_node):
    global allowed_hom_types, node_name_to_uid
    if hom_node.inputs():
        input_node = hom_node.inputs()[0]
        parent_name = input_node.name()
        if parent_name not in node_name_to_uid.keys():
            return 0
        return node_name_to_uid[parent_name]
    return 0

# get all hom nodes of the needed type
obj_nodes = [node for node in hou.node("/obj").children() if node.type().name() in allowed_hom_types]
meshes = {}
materials = {}

nodes = []

for node in obj_nodes:
    entity = Node("Entity")
    nodes.append(entity)
    uid = entity.unique_id
    uid_to_hom_node[uid] = node
    node_name_to_uid[node.name()] = uid
    uid_to_components[uid] = []

for uid, hom_node in uid_to_hom_node.items():
    # add object metadata component
    current_node = Node("ObjectMetadata")
    current_node.attributes.append(Attribute("Name", DataType_CHAR, hom_node.name(), True))
    current_node.attributes.append(Attribute("Tag", DataType_CHAR, "default", True))
    current_node.attributes.append(Attribute("Layer", DataType_CHAR, "default", True))
    components.append(current_node)
    uid_to_components[uid].append(current_node.unique_id)
    # add transform component
    # calculate transforms from the hom node
    local_transform = hom_node.localTransform()
    position = local_transform.extractTranslates()
    quat = hou.Quaternion(local_transform.extractRotationMatrix3())
    scale = local_transform.extractScales()
    # transform node
    current_node = Node("Transform")
    current_node.attributes.append(Attribute("ParentID", DataType_UID, parent_node_uid(hom_node), True))
    current_node.attributes.append(Attribute("IsDynamic", DataType_BYTE, 0, True))
    current_node.attributes.append(Attribute("LocalPosition", DataType_FLOAT, position))
    current_node.attributes.append(Attribute("LocalRotation", DataType_FLOAT, quat))
    current_node.attributes.append(Attribute("LocalScale", DataType_FLOAT, scale))
    components.append(current_node)
    uid_to_components[uid].append(current_node.unique_id)
    # handle light component
    if hom_node.type().name() == light_type_name:
        current_node = Node("Light")
        current_node.attributes.append(Attribute("Color", DataType_FLOAT, hom_node.parmTuple("light_color").eval()))
        current_node.attributes.append(Attribute("Exposure", DataType_FLOAT, hom_node.parm("light_exposure").eval(), True))
        current_node.attributes.append(Attribute("Intensity", DataType_FLOAT, hom_node.parm("light_intensity").eval(), True))
        components.append(current_node)
        uid_to_components[uid].append(current_node.unique_id)
    # handle mesh component
    if hom_node.type().name() == geo_type_name:
        geometry_data = GeometryData(hom_node)
        # make mesh asset
        geometry_node = Node("Mesh")
        geometry_node.attributes.append(Attribute("Name", DataType_CHAR, hom_node.name(), True))
        geometry_node.attributes.append(Attribute("Indices", DataType_UINT, geometry_data.indices))
        geometry_node.attributes.append(Attribute("Normals", DataType_FLOAT, geometry_data.normals))
        geometry_node.attributes.append(Attribute("Positions", DataType_FLOAT, geometry_data.positions))
        geometry_node.attributes.append(Attribute("TexCoord", DataType_FLOAT, geometry_data.texcoord))
        meshes[hom_node.name()] = geometry_node
        # add renderer component
        renderer_node = Node("Renderer")
        renderer_node.attributes.append(Attribute("DrawType", DataType_BYTE, DrawType_FILL, True))
        renderer_node.attributes.append(Attribute("Enabled", DataType_BYTE, 1, True))
        renderer_node.attributes.append(Attribute("MeshReference", DataType_UID, geometry_node.unique_id, True))
        # find material
        material_node = None
        # find material
        material_id = 0
        if hom_node.parm("shop_materialpath").eval():
            material_hom_node = hou.node(hom_node.parm("shop_materialpath").eval())
            if material_hom_node.type().name() == "principledshader::2.0":
                material_name = material_hom_node.name()
                if material_name not in materials.keys():
                    material_node = Node("Material")
                    material_node.attributes.append(Attribute("Name", DataType_CHAR, material_name, True))
                    material_node.attributes.append(Attribute("ShaderId", DataType_UID, 0, True))
                    material_node.attributes.append(Attribute("DiffuseColor", DataType_FLOAT, hom_node.parmTuple("basecolor").eval()))
                    materials[material_name] = material_node
                    material_id = material_node.unique_id
                else:
                    material_id = materials[material_name]
        renderer_node.attributes.append(Attribute("MaterialReference", DataType_UID, material_id, True))
        components.append(renderer_node)
        uid_to_components[uid].append(renderer_node.unique_id)
# create nodes collection
# make entity/component id collection
id_collection_node = Node("EntityIdCollection")
id_collection_node.attributes.append(Attribute("Ids", DataType_UID, uid_to_components.keys()))
comid_collection = []
for uid, component_ids in uid_to_components.items():
    cmid = Node("ComponentIdCollection")
    cmid.attributes.append(Attribute("Ids", DataType_UID, uid_to_components[uid]))
    comid_collection.append(cmid)
id_collection_node.attributes.append(Attribute("Components", DataType_SERIALIZED_CLASS, comid_collection))
nodes.append(id_collection_node)
# add components
nodes.extend(components)
# add mesh assets
nodes.extend(meshes.values())
# add material assets
nodes.extend(materials.values())
# add test shader asset
shader_node = Node("Shader")
shader_node.attributes.append(Attribute("Language", DataType_CHAR, "GLSL", True))
shader_source_vertex = Node("ShaderSource")
shader_source_vertex.attributes.append(Attribute("Type", DataType_UINT16, 1, True))
shader_source_vertex.attributes.append(Attribute("Source", DataType_CHAR, "int main() \n { \n return 0; \n }", True))
shader_source_fragment = Node("ShaderSource")
shader_source_fragment.attributes.append(Attribute("Type", DataType_UINT16, 1, True))
shader_source_fragment.attributes.append(Attribute("Source", DataType_CHAR, "int mainMain() \n { \n int x = 2; \n return 0; \n }", True))
shader_node.attributes.append(Attribute("Source", DataType_SERIALIZED_CLASS, [shader_source_vertex, shader_source_fragment]))
nodes.append(shader_node)

version = 1
signature = "SSD"
node_begin = 0xaa
node_end = 0xab
attr_begin = 0xba
attr_end = 0xbb
character_limit = 32

def get_uint16_to_bytes(val):
    val = val & 0xffff
    return ((val >> 8) & 0xff, val & 0xff)

def get_uint32_to_bytes(val):
    val = val & 0xffffffff
    return (
        (val >> 24) & 0xff, 
        (val >> 16) & 0xff, 
        (val >> 8) & 0xff, 
        val & 0xff)

def string_to_bytes(s):
    ba = bytearray(s)
    ba.append(0)
    return ba
    
def attribute_to_bytes(attr):
    attr_data = bytearray()
    attr_data.append(attr_begin)
    attr_name = string_to_bytes(attr.name[:255])
    attr_data.append(len(attr_name) & 0xff)
    attr_data.extend(attr_name)

    # data type name

    data_type_name = string_to_bytes(data_type_name_map[attr.data_type])
    attr_data.append(len(data_type_name) & 0xff)
    attr_data.extend(data_type_name)
    # attr_data
    single_element_value = 0 # attr.single_element
    
    if attr.single_element:
        single_element_value = 1

    attr_data.append(single_element_value & 0xff)

    # handle strings
    if attr.data_type == DataType_CHAR:
        if attr.single_element:
            # byte count
            attr_data.extend(get_uint32_to_bytes(len(attr.value) + 1))
            # element count
            attr_data.extend(get_uint32_to_bytes(1))
            # value
            attr_data.extend(attr.value)
            attr_data.append('\0')
        else:
            element_count = len(attr.value)
            values = bytearray()
            for x in range(element_count):
                values.extend(attr.value[x])
                values.append('\0')
            # byte count
            attr_data.extend(get_uint32_to_bytes(len(values)))
            # element count
            attr_data.extend(get_uint32_to_bytes(element_count))
            # value
            attr_data.extend(values)
    elif attr.data_type == DataType_SERIALIZED_CLASS:
        element_count = len(attr.value)
        # byte count
        attr_data.extend(get_uint32_to_bytes(element_count))
        # element count
        attr_data.extend(get_uint32_to_bytes(element_count))
        # value
        for c in attr.value:
            attr_data.extend(node_to_bytes(c))
    else:
        values = attr.value
        if attr.single_element:
            element_count = 1
            values = [attr.value,]
        element_count = len(values)
        byte_count = element_count
        unpacked_values = bytearray()
        for x in range(len(values)):
            if (attr.data_type == DataType_FLOAT):
                byte_count = element_count * 4
                unpacked_values.extend(bytearray(struct.pack("f", values[x])))
            if (attr.data_type == DataType_UINT or attr.data_type == DataType_UID \
                or attr.data_type == DataType_INT):
                byte_count = element_count * 4
                unpacked_values.extend(get_uint32_to_bytes(values[x]))
            if (attr.data_type == DataType_UINT16 or attr.data_type == DataType_INT16):
                byte_count = element_count * 2
                unpacked_values.extend(get_uint16_to_bytes(values[x]))
            if (attr.data_type == DataType_BYTE):
                unpacked_values.append(values[x] & 0xff)
        # byte count
        attr_data.extend(get_uint32_to_bytes(byte_count))
        # element count
        attr_data.extend(get_uint32_to_bytes(element_count))
        # value
        attr_data.extend(unpacked_values)

    attr_data.append(attr_end)
    return attr_data

def node_to_bytes(node):
    node_data = bytearray()
    node_data.append(node_begin)
    node_data.extend(get_uint32_to_bytes(node.unique_id))
    # node_data.append(node.type & 0xff)

    node_name = string_to_bytes(node.name[:255])
    node_data.append(len(node_name) & 0xff)
    node_data.extend(node_name)

    node_data.append(len(node.attributes) & 0xff)
    node_data.append(len(node.nodes) & 0xff)

    for x in range(len(node.attributes)):
        node_data.extend(attribute_to_bytes(node.attributes[x]))
    for x in range(len(node.nodes)):
        node_data.extend(node_to_bytes(node.nodes[x]))
    node_data.append(node_end)
    return node_data

# file select
out_file = hou.expandString(hou.ui.selectFile(pattern="*.ssd"))

if not out_file.endswith(".ssd"):
    out_file += ".ssd"

data = bytearray(signature)
data.append(version)
data.extend(get_uint16_to_bytes(len(nodes)))

for node in nodes:
    # write nodes
    data.extend(node_to_bytes(node))
    
f = open(out_file, "w+b")
f.write(data)
f.close()
