from . import core
import struct

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

    data_type_name = string_to_bytes(core.data_type_name_map[attr.data_type])
    attr_data.append(len(data_type_name) & 0xff)
    attr_data.extend(data_type_name)
    # attr_data
    single_element_value = 0 # attr.single_element
    
    if attr.single_element:
        single_element_value = 1

    attr_data.append(single_element_value & 0xff)

    # handle strings
    if attr.data_type == core.DataType_CHAR:
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
    elif attr.data_type == core.DataType_SERIALIZED_CLASS:
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
            if (attr.data_type == core.DataType_FLOAT):
                byte_count = element_count * 4
                unpacked_values.extend(bytearray(struct.pack("f", values[x])))
            if (attr.data_type == core.DataType_UINT or attr.data_type == core.DataType_UID \
                or attr.data_type == core.DataType_INT):
                byte_count = element_count * 4
                unpacked_values.extend(get_uint32_to_bytes(values[x]))
            if (attr.data_type == core.DataType_UINT16 or attr.data_type == core.DataType_INT16):
                byte_count = element_count * 2
                unpacked_values.extend(get_uint16_to_bytes(values[x]))
            if (attr.data_type == core.DataType_BYTE):
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

def make_byte_header():
    data = bytearray(signature)
    data.append(version)
    return data

def node_list_to_bytes(nodes):
    data = make_byte_header()
    data.extend(get_uint16_to_bytes(len(nodes)))
    for node in nodes:
        # write nodes
        data.extend(node_to_bytes(node))
    return data
    