from . import core
from . import mesh_parser
import hou

class Exporter:
    def __init__(self):
        self.meshes = {}
        self.materials = {}
        self.nodes = []  
        self.node_name_to_uid = {}
        self.uid_to_hom_node = {}
        self.uid_to_components = {}
        self.components = []

    def get_nodes(self):
        self.initialize()
        self.set_parent_ids()
        self.collect_data_from_hom_nodes()
        self.create_node_list_from_collected_data()
        return self.nodes

    def parent_node_uid(self, hom_node):
        if hom_node.inputs():
            input_node = hom_node.inputs()[0]
            parent_name = input_node.name()
            if parent_name not in self.node_name_to_uid.keys():
                return 0
            return self.node_name_to_uid[parent_name]
        return 0

    def initialize(self):
        core.initialize_global_id()
        self.meshes = {}
        self.materials = {}
        self.nodes = []  
        self.node_name_to_uid = {}
        self.uid_to_hom_node = {}
        self.uid_to_components = {}
        self.components = []

    def set_parent_ids(self):
        # reset main containers
        obj_nodes = [node for node in hou.node("/obj").children() 
            if node.type().name() in core.allowed_hom_types]

        # set dummy entity nodes
        for node in obj_nodes:
            entity = core.Node("Entity")
            self.nodes.append(entity)
            uid = entity.unique_id
            self.uid_to_hom_node[uid] = node
            self.node_name_to_uid[node.name()] = uid
            self.uid_to_components[uid] = []

    def create_node_list_from_collected_data(self):
        # create nodes collection
        # make entity/component id collection
        id_collection_node = core.Node("EntityIdCollection")
        id_collection_node.attributes.append(core.Attribute("Ids", core.DataType_UID, self.uid_to_components.keys()))
        comid_collection = []
        for uid, component_ids in self.uid_to_components.items():
            cmid = core.Node("ComponentIdCollection")
            cmid.attributes.append(core.Attribute("Ids", core.DataType_UID, self.uid_to_components[uid]))
            comid_collection.append(cmid)
        id_collection_node.attributes.append(core.Attribute("Components", core.DataType_SERIALIZED_CLASS, comid_collection))
        self.nodes.append(id_collection_node)
        # add components
        self.nodes.extend(self.components)
        # add mesh assets
        self.nodes.extend(self.meshes.values())
        # add material assets
        self.nodes.extend(self.materials.values())
        # add test shader asset
        shader_node = core.Node("Shader")
        shader_node.attributes.append(core.Attribute("Language", core.DataType_CHAR, "GLSL", True))
        shader_source_vertex = core.Node("ShaderSource")
        shader_source_vertex.attributes.append(core.Attribute("Type", core.DataType_UINT16, 1, True))
        shader_source_vertex.attributes.append(core.Attribute("Source", core.DataType_CHAR, "int main() \n { \n return 0; \n }", True))
        shader_source_fragment = core.Node("ShaderSource")
        shader_source_fragment.attributes.append(core.Attribute("Type", core.DataType_UINT16, 1, True))
        shader_source_fragment.attributes.append(core.Attribute("Source", core.DataType_CHAR, "int mainMain() \n { \n int x = 2; \n return 0; \n }", True))
        shader_node.attributes.append(core.Attribute("Source", core.DataType_SERIALIZED_CLASS, [shader_source_vertex, shader_source_fragment]))
        self.nodes.append(shader_node)

    def collect_data_from_hom_nodes(self):
        # go through hom nodes
        for uid, hom_node in self.uid_to_hom_node.items():
            # add object metadata component
            current_node = core.Node("ObjectMetadataComponent")
            current_node.attributes.append(core.Attribute("Name", core.DataType_CHAR, hom_node.name(), True))
            current_node.attributes.append(core.Attribute("Tag", core.DataType_CHAR, "default", True))
            current_node.attributes.append(core.Attribute("Layer", core.DataType_CHAR, "default", True))
            self.components.append(current_node)
            self.uid_to_components[uid].append(current_node.unique_id)
            # add transform component
            # calculate transforms from the hom node
            local_transform = hom_node.localTransform()
            position = local_transform.extractTranslates()
            quat = hou.Quaternion(local_transform.extractRotationMatrix3())
            scale = local_transform.extractScales()
            # transform node
            current_node = core.Node("TransformComponent")
            current_node.attributes.append(core.Attribute("ParentID", core.DataType_UID, self.parent_node_uid(hom_node), True))
            current_node.attributes.append(core.Attribute("IsDynamic", core.DataType_BYTE, 0, True))
            current_node.attributes.append(core.Attribute("LocalPosition", core.DataType_FLOAT, position))
            current_node.attributes.append(core.Attribute("LocalRotation", core.DataType_FLOAT, quat))
            current_node.attributes.append(core.Attribute("LocalScale", core.DataType_FLOAT, scale))
            self.components.append(current_node)
            self.uid_to_components[uid].append(current_node.unique_id)
            # handle light component
            if hom_node.type().name() == core.light_type_name:
                current_node = core.Node("LightComponent")
                current_node.attributes.append(core.Attribute("Color", core.DataType_FLOAT, hom_node.parmTuple("light_color").eval()))
                current_node.attributes.append(core.Attribute("Exposure", core.DataType_FLOAT, hom_node.parm("light_exposure").eval(), True))
                current_node.attributes.append(core.Attribute("Intensity", core.DataType_FLOAT, hom_node.parm("light_intensity").eval(), True))
                self.components.append(current_node)
                self.uid_to_components[uid].append(current_node.unique_id)
            # handle mesh component
            if hom_node.type().name() == core.geo_type_name:
                geometry_data = mesh_parser.GeometryData(hom_node)
                geometry_node = geometry_data.get_parsed_mesh_node(hom_node.name())
                self.meshes[hom_node.name()] = geometry_node
                # add renderer component
                renderer_node = core.Node("RendererComponent")
                renderer_node.attributes.append(core.Attribute("DrawType", core.DataType_BYTE, core.DrawType_FILL, True))
                renderer_node.attributes.append(core.Attribute("Enabled", core.DataType_BYTE, 1, True))
                renderer_node.attributes.append(core.Attribute("MeshReference", core.DataType_UID, geometry_node.unique_id, True))
                # find material
                material_node = None
                # find material
                material_id = 0
                if hom_node.parm("shop_materialpath").eval():
                    material_hom_node = hou.node(hom_node.parm("shop_materialpath").eval())
                    if material_hom_node:
                        if material_hom_node.type().name() == "principledshader::2.0":
                            material_name = material_hom_node.name()
                            if material_name not in self.materials.keys():
                                material_node = core.Node("Material")
                                material_node.attributes.append(core.Attribute("Name", core.DataType_CHAR, material_name, True))
                                material_node.attributes.append(core.Attribute("ShaderId", core.DataType_UID, 0, True))
                                material_node.attributes.append(core.Attribute("DiffuseColor", core.DataType_FLOAT, hom_node.parmTuple("basecolor").eval()))
                                self.materials[material_name] = material_node
                                material_id = material_node.unique_id
                            else:
                                material_id = self.materials[material_name].unique_id
                renderer_node.attributes.append(core.Attribute("MaterialReference", core.DataType_UID, material_id, True))
                self.components.append(renderer_node)
                self.uid_to_components[uid].append(renderer_node.unique_id)
                
