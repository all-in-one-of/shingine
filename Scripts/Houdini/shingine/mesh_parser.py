
import hou
from . import core

class GeometryData:
    def __init__(self, node):
        self.nodes_to_destroy = []
        display = node.displayNode()
        display = self.find_texture_coordinates(display)
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
            texture_coordinate_value = point.attribValue("uv")
            texcoord.append(texture_coordinate_value[0])
            texcoord.append(texture_coordinate_value[1])
            texcoord.append(texture_coordinate_value[2])
        for face in geometry.prims():
            vertices = reversed(face.vertices())
            for vert in vertices:
                indices.append(vert.point().number())
        # delete temp houdini nodes
        self.nodes_to_destroy.extend([normal, divide])
        for node in self.nodes_to_destroy:
            node.destroy()
            
        self.positions = positions
        self.normals = normals
        self.indices = indices
        self.texcoord = texcoord

    def find_texture_coordinates(self, sop_node):
        # check if has uvs
        node = sop_node.parent()
        new_node = sop_node
        if sop_node.geometry().findPointAttrib("uv"):
            return sop_node
        # add uv unwrap
        if not sop_node.geometry().findVertexAttrib("uv"):
            uvunwrap = node.createNode("uvunwrap")
            self.nodes_to_destroy.append(uvunwrap)
            uvunwrap.setParms({"uvattrib" : "uv"})
            uvunwrap.setInput(0, new_node)
            new_node = uvunwrap
        # promote vertex uv attribute to the point one
        attribpromote = node.createNode("attribpromote")
        self.nodes_to_destroy.append(attribpromote)
        attribpromote.setParms({"inname" : "uv", "inclass" : 3, "outclass" : 2})
        attribpromote.setInput(0, new_node)
        new_node = attribpromote
        return new_node

    def get_parsed_mesh_node(self, name="GenericMeshNode"):
        geometry_node = core.Node("Mesh")
        geometry_node.attributes.append(
            core.Attribute("Name", core.DataType_CHAR, name, True))
        geometry_node.attributes.append(
            core.Attribute("Indices", core.DataType_UINT, self.indices))
        geometry_node.attributes.append(
            core.Attribute("Normals", core.DataType_FLOAT, self.normals))
        geometry_node.attributes.append(
            core.Attribute("Positions", core.DataType_FLOAT, self.positions))
        geometry_node.attributes.append(
            core.Attribute("TexCoord", core.DataType_FLOAT, self.texcoord))
        return geometry_node
        
def mesh_nodes_from_hom_nodes(hom_nodes):
    nodes = []
    for hom_node in hom_nodes:
        geometry_data = GeometryData(hom_node)
        nodes.append(geometry_data.get_parsed_mesh_node)
