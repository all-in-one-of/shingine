from . import core
from . import mesh_parser
from . import node_management
from . import binary_writer
import hou
import os


class Exporter:
    def __init__(self, base_path):
        self.base_dir = ""
        self.asset_dir = ""
        self.meshes_dir = ""
        self.materials_dir = ""

        self.meshes = {}
        self.materials = {}
        self.nodes = []
        self.node_name_to_uid = {}
        self.uid_to_hom_node = {}
        self.uid_to_components = {}
        self.components = []

        self.external_assets_flag = False

        self.set_directories(base_path)

    def set_directories(self, base_path):
        self.base_dir = base_path
        self.asset_dir = "SceneAssets"
        self.meshes_dir = "Meshes"
        self.materials_dir = "Materials"

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
        id_collection_node.attributes.append(
            core.Attribute(
                "Ids", core.DataType_UID, self.uid_to_components.keys()))
        comid_collection = []
        for uid in self.uid_to_components.keys():
            cmid = core.Node("ComponentIdCollection")
            cmid.attributes.append(
                core.Attribute(
                    "Ids", core.DataType_UID, self.uid_to_components[uid]))
            comid_collection.append(cmid)
        id_collection_node.attributes.append(
            core.Attribute(
                "Components", core.DataType_SERIALIZED_CLASS, comid_collection))
        self.nodes.append(id_collection_node)
        # add components
        self.nodes.extend(self.components)
        # add mesh assets
        self.nodes.extend(self.meshes.values())
        # add material assets
        self.nodes.extend(self.materials.values())

    def collect_data_from_hom_nodes(self):
        for uid, hom_node in self.uid_to_hom_node.items():
            if hom_node.parm(node_management.external_export_parm_name):
                self.external_assets_flag = True
            # object metadata
            self.add_component(uid,
                               node_management.get_object_metadata_node(hom_node.name()))
            # transform
            self.add_component(uid,
                               node_management.get_transform_component_node(
                                   self.parent_node_uid(hom_node),
                                   self.calculate_transform_values(hom_node)))
            # light
            self.find_light_component(uid, hom_node)
            # handle mesh component
            self.find_geometry(uid, hom_node)
            self.external_assets_flag = False

    def add_component(self, uid, component_node):
        self.components.append(component_node)
        self.uid_to_components[uid].append(component_node.unique_id)

    def calculate_transform_values(self, hom_node):
        local_transform = hom_node.localTransform()
        position = local_transform.extractTranslates()
        quat = hou.Quaternion(local_transform.extractRotationMatrix3())
        scale = local_transform.extractScales()
        return position, quat, scale

    def find_light_component(self, uid, hom_node):
        if not hom_node.type().name() == core.light_type_name:
            return
        light_node = node_management.get_light_component_node(
            hom_node.parmTuple("light_color").eval(),
            hom_node.parm("light_exposure").eval(),
            hom_node.parm("light_intensity").eval()
        )
        self.add_component(uid, light_node)

    def find_geometry(self, uid, hom_node):
        if not hom_node.type().name() == core.geo_type_name:
            return
        geometry_data = mesh_parser.GeometryData(hom_node)
        geometry_node = geometry_data.get_parsed_mesh_node(hom_node.name())
        geometry_node.attributes.append(core.Attribute(
            "Name", core.DataType_CHAR, hom_node.name(), True))
        self.meshes[hom_node.name()] = self.get_asset(geometry_node)
        # add renderer component
        renderer_node = node_management.get_renderer_component_node(
            geometry_node.unique_id)
        # find material
        material_id, material_name, material_node = self.find_material_id(
            hom_node)

        if material_node:
            self.materials[material_name] = self.get_asset(material_node)

        renderer_node.attributes.append(
            core.Attribute(
                "MaterialReference", core.DataType_UID, material_id, True))
        self.add_component(uid, renderer_node)

    def get_asset(self, node):
        # if it's not external, return the regular node
        if not self.external_assets_flag:
            return node
        
        external_node = None
        # generate the asset name or use from the attribute
        asset_name = node.name + "_" + str(node.unique_id)
        attr = core.get_attribute(node, "Name")
        if attr:
            asset_name = attr.value

        # path relative to "SceneAssets directory"
        asset_dir = ""
        # Set specific directory to the asset type
        if node.name == node_management.shingine_material_node_type:
            asset_dir = self.materials_dir
        elif node.name == node_management.shingine_mesh_node_type:
            asset_dir = self.meshes_dir

        # create a full directory path, ensure all folders are created
        full_path = os.path.join(self.base_dir, self.asset_dir, asset_dir)
        if not os.path.exists(full_path):
            os.makedirs(full_path)

        # info file name is necessary to include the asset type to the file name
        asset_filename = asset_name + core.file_name_extension
        info_asset_filename = os.path.join(asset_dir,asset_filename)
        # this external node will be saved to the scene file
        external_node = node_management.get_external_node(node.name,
                                                          info_asset_filename)

        # write the original node in the external file
        full_path = os.path.join(full_path, asset_filename)
        node_data = binary_writer.node_list_to_bytes((node,))
        f = open(full_path, "w+b")
        f.write(node_data)
        f.close()

        return external_node

    def find_material_id(self, hom_node):
        material_node = None
        material_name = ""
        material_id = 0
        default_tuple = (material_id, material_name, material_node)
        if not hom_node.parm("shop_materialpath").eval():
            return default_tuple

        material_hom_node = hou.node(
            hom_node.parm("shop_materialpath").eval())
        if not material_hom_node:
            return default_tuple

        if not material_hom_node.type().name() == "principledshader::2.0":
            return default_tuple

        material_name = material_hom_node.name()
        if material_name not in self.materials.keys():
            material_node = node_management.get_material_node(
                material_name,
                hom_node.parmTuple("basecolor").eval())
            material_id = material_node.unique_id
        else:
            material_id = self.materials[material_name].unique_id

        return material_id, material_name, material_node
