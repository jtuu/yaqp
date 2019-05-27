import sys
import common

mon_names = [
    "Hildebear",
    "Hildeblue",
    "Rag Rappy",
    "Al Rappy",
    "Monest",
    "Booma",
    "Gobooma",
    "Gigobooma",
    "Savage Wolf",
    "Barbarous Wolf",
    "Grass Assassin",
    "Poison Lily",
    "Nar Lily",
    "Nano Dragon",
    "Evil Shark",
    "Pal Shark",
    "Guil Shark",
    "Pofuilly Slime",
    "Pouilly Slime",
    "Pan Arms",
    "Migium",
    "Hidoom",
    "Gillchic",
    "Dubchic",
    "Garanz",
    "Sinow Beat",
    "Sinow Gold",
    "Canadine",
    "Canane",
    "Dubwitch",
    "Delsaber",
    "Chaos Sorcerer",
    "Dark Gunner",
    "Death Gunner",
    "Chaos Bringer",
    "Dark Belra",
    "Dimenian",
    "La Dimenian",
    "So Dimenian",
    "Bulclaw",
    "Claw",
    "Dragon",
    "Gal Gryphon",
    "De Rol Le",
    "Vol Opt",
    "Dark Falz",
    "Olga Flow",
    "Barba Ray",
    "Gol Dragon",
    "Love Rappy",
    "Sinow Berill",
    "Sinow Spigell",
    "Merillia",
    "Meriltas",
    "Mericus",
    "Merikle",
    "Mericarol",
    "Ul Gibbon",
    "Zol Gibbon",
    "Gibbles",
    "Gee",
    "Gi Gue",
    "Deldepth",
    "Delbiter",
    "Dolmolm",
    "Dolmdarl",
    "Morfos",
    "Recobox",
    "Recon",
    "Sinow Zoa",
    "Sinow Zele",
    "Epsilon",
    "Del Lily",
    "Ill Gill",
    "Sand Rappy",
    "Del Rappy",
    "Astark",
    "Satellite Lizard",
    "Yowie",
    "Merissa A",
    "Merissa AA",
    "Girtablulu",
    "Zu",
    "Pazuzu",
    "Boota",
    "Ze Boota",
    "Ba Boota",
    "Dorphon",
    "Dorphon Eclair",
    "Goran",
    "Pyro Goran",
    "Goran Detonator",
    "Saint-Milion",
    "Shambertin",
    "Kondrieu",
    "Unknown",
    "Ignore"
]

if __name__ == "__main__":
    (h_path, c_path) = common.make_file_paths(sys.argv[-1])

    h_contents = ""
    c_contents = ""
    num_def_name = "NUM_NPCS"
    map_name = "npc_names"
    prefix = "MON_"

    h_contents += common.make_enum("mon_kind", mon_names, prefix)
    h_contents += common.make_name_map_declaration(num_def_name, map_name, mon_names)
    c_contents += common.make_name_map_definition(num_def_name, map_name, mon_names, prefix)
    
    h_contents = common.wrap_guard(__file__, "YAQP_MON_H", h_contents)
    c_contents = common.add_src_header(h_path, c_contents)
    c_contents = common.add_comment(__file__, c_contents)
    
    with open(h_path, "w") as f:
        f.write(h_contents)
    with open(c_path, "w") as f:
        f.write(c_contents)
