
import hou
from . import core

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

    def get_parsed_mesh_node(self, name="GenericMeshNode"):
        geometry_node = core.Node("Mesh")
        geometry_node.attributes.append(core.Attribute("Name", core.DataType_CHAR, name, True))
        geometry_node.attributes.append(core.Attribute("Indices", core.DataType_UINT, self.indices))
        geometry_node.attributes.append(core.Attribute("Normals", core.DataType_FLOAT, self.normals))
        geometry_node.attributes.append(core.Attribute("Positions", core.DataType_FLOAT, self.positions))
        geometry_node.attributes.append(core.Attribute("TexCoord", core.DataType_FLOAT, self.texcoord))
        return geometry_node
        
def mesh_nodes_from_hom_nodes(hom_nodes):
    nodes = []
    for hom_node in hom_nodes:
        geometry_data = GeometryData(hom_node)
        nodes.append(geometry_data.get_parsed_mesh_node)
