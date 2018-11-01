import hou
from . import core
external_export_parm_name = "shingine_external_export"

shingine_transform_component_type = "TransformComponent"
shingine_object_metadata_component_type = "ObjectMetadataComponent"
shingine_renderer_component_type = "RendererComponent"
shingine_light_component_type = "LightComponent"
shingine_mesh_node_type = "Mesh"
shingine_material_node_type = "Material"
shingine_shade_node_type = "Shader"
shingine_shade_source_node_type = "ShaderSource"

shingine_external_node_type = "ExternalAsset"


def add_external_export_flag():
    nodes = hou.selectedNodes()
    for node in nodes:
        if node.parm(external_export_parm_name):
            continue
        parm_group = node.parmTemplateGroup()
        parm_folder = hou.FolderParmTemplate("folder", "shingine")
        parm_folder.addParmTemplate(hou.FloatParmTemplate(
            external_export_parm_name, "External Export", 1))
        parm_group.append(parm_folder)
        node.setParmTemplateGroup(parm_group)

def get_mesh_node():
    node = core.Node(shingine_mesh_node_type)
    return node

def get_external_node(uid, node_name, filename):
    node = core.Node(shingine_external_node_type, uid)
    node.attributes.append(core.Attribute(
        "Name", core.DataType_CHAR, node_name, True))
    node.attributes.append(core.Attribute(
        "FileName", core.DataType_CHAR, filename, True))
    return node

def get_material_node(material_name, color):
    node = core.Node(shingine_material_node_type)
    node.attributes.append(core.Attribute(
        "Name", core.DataType_CHAR, material_name, True))
    node.attributes.append(core.Attribute(
        "ShaderId", core.DataType_UID, 0, True))
    node.attributes.append(core.Attribute(
        "DiffuseColor", core.DataType_FLOAT, color))
    return node


def get_renderer_component_node(geometry_id):
    node = core.Node(shingine_renderer_component_type)
    node.attributes.append(core.Attribute(
        "DrawType", core.DataType_BYTE, core.DrawType_FILL, True))
    node.attributes.append(core.Attribute(
        "Enabled", core.DataType_BYTE, 1, True))
    node.attributes.append(core.Attribute(
        "MeshReference", core.DataType_UID, geometry_id, True))
    return node


def get_vertex_fragment_source_node(vert_source, frag_source, shader_language_name="glsl"):
    shader_node = core.Node(shingine_shade_node_type)
    shader_node.attributes.append(core.Attribute(
        "Language", core.DataType_CHAR, shader_language_name, True))
    shader_source_vertex = core.Node(shingine_shade_source_node_type)
    shader_source_vertex.attributes.append(
        core.Attribute("Type", core.DataType_UINT16, 1, True))
    shader_source_vertex.attributes.append(core.Attribute(
        "Source", core.DataType_CHAR, vert_source, True))
    shader_source_fragment = core.Node(shingine_shade_source_node_type)
    shader_source_fragment.attributes.append(
        core.Attribute("Type", core.DataType_UINT16, 1, True))
    shader_source_fragment.attributes.append(core.Attribute(
        "Source", core.DataType_CHAR, frag_source, True))
    shader_node.attributes.append(core.Attribute("Source", core.DataType_SERIALIZED_CLASS, [
                                  shader_source_vertex, shader_source_fragment]))
    return shader_node


def get_light_component_node(light_color, light_exposure, light_intensity):
    current_node = core.Node(shingine_light_component_type)
    current_node.attributes.append(core.Attribute(
        "Color", core.DataType_FLOAT, light_color))
    current_node.attributes.append(core.Attribute(
        "Exposure", core.DataType_FLOAT, light_exposure, True))
    current_node.attributes.append(core.Attribute(
        "Intensity", core.DataType_FLOAT, light_intensity, True))
    return current_node


def get_transform_component_node(parentId, params):
    (position, quat, scale) = params
    node = core.Node(shingine_transform_component_type)
    node.attributes.append(core.Attribute(
        "ParentID", core.DataType_UID, parentId, True))
    node.attributes.append(core.Attribute(
        "IsDynamic", core.DataType_BYTE, 0, True))
    node.attributes.append(core.Attribute(
        "LocalPosition", core.DataType_FLOAT, position))
    node.attributes.append(core.Attribute(
        "LocalRotation", core.DataType_FLOAT, quat))
    node.attributes.append(core.Attribute(
        "LocalScale", core.DataType_FLOAT, scale))
    return node


def get_object_metadata_node(name):
    new_node = core.Node(shingine_object_metadata_component_type)
    new_node.attributes.append(core.Attribute(
        "Name", core.DataType_CHAR, name, True))
    new_node.attributes.append(core.Attribute(
        "Tag", core.DataType_CHAR, "default", True))
    new_node.attributes.append(core.Attribute(
        "Layer", core.DataType_CHAR, "default", True))
    return new_node
