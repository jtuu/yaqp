import os
import sys
import common

ep1_area_names = [
    "Pioneer 2",
    "Forest 1",
    "Forest 2",
    "Caves 1",
    "Caves 2",
    "Caves 3",
    "Mines 1",
    "Mines 2",
    "Ruins 1",
    "Ruins 2",
    "Ruins 3",
    "Under the Dome",
    "Underground Channel",
    "Monitor Room",
    "????",
    "Visual Lobby",
    "VR Spaceship Alpha",
    "VR Temple Alpha"
]

ep2_area_names = [
    "Lab",
    "VR Temple Alpha",
    "VR Temple Beta",
    "VR Spaceship Alpha",
    "VR Spaceship Beta",
    "Central Control Area",
    "Jungle North",
    "Jungle East",
    "Mountain",
    "Seaside",
    "Seabed Upper",
    "Seabed Lower",
    "Cliffs of Gal Da Val",
    "Test Subject Disposal Area",
    "VR Temple Final",
    "VR Spaceship Final",
    "Seaside Night",
    "Control Tower"
]

ep4_area_names = [
    "Pioneer 2",
    "Crater East",
    "Crater West",
    "Crater South",
    "Crater North",
    "Crater Interior",
    "Subterranean Desert 1",
    "Subterranean Desert 2",
    "Subterranean Desert 3",
    "Meteor Impact Site",
    "Caves 2"
]

area_names = {
    1: ep1_area_names,
    2: ep2_area_names,
    4: ep4_area_names
}

if __name__ == "__main__":
    (h_path, c_path) = common.make_file_paths(sys.argv[-1])

    h_contents = ""
    c_contents = ""

    for ep in area_names:
        num_def_name = "NUM_EP%d_AREAS" % (ep)
        var_name = "ep%d_area_names" % (ep)
        names = area_names[ep]
        prefix = "EP%d_" % (ep)
        h_contents += common.make_enum("ep%d_area" % (ep), names, prefix)
        h_contents += common.make_name_map_declaration(num_def_name, var_name, names)
        c_contents += common.make_name_map_definition(num_def_name, var_name, names, prefix)
    
    h_contents = common.wrap_guard(__file__, "YAQP_AREA_H", h_contents)
    c_contents = common.add_src_header(h_path, c_contents)
    c_contents = common.add_comment(__file__, c_contents)
    
    with open(h_path, "w") as f:
        f.write(h_contents)
    with open(c_path, "w") as f:
        f.write(c_contents)
