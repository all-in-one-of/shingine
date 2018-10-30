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

def initialize_global_id():
    global NextId
    NextId = 50

def GetUid():
    global NextId
    uid = NextId
    NextId += 1
    return uid

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
