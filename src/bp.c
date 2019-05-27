#include "mon.h"
#include "bp.h"

mon_kind ep1_bp_ids[NUM_EP1_BP] = {
    MON_IGNORE, 		// Mothmant
    MON_MONEST, 		// Monest
    MON_SAVAGE_WOLF, 	// Savage Wolf
    MON_BARBAROUS_WOLF, // Barbarous Wolf
    MON_POISON_LILY, 	// Poison Lily
    MON_NAR_LILY, 		// Nar Lily
    MON_SINOW_BEAT, 	// Sinow Beat
    MON_CANADINE, 		// Canadine
    MON_IGNORE, 		// Canadine (Ring)
    MON_CANANE, 		// Canane
    MON_CHAOS_SORCERER, // Chaos Sorcerer
    MON_IGNORE, 		// Bee R
    MON_IGNORE, 		// Bee L
    MON_CHAOS_BRINGER, 	// Chaos Bringer
    MON_DARK_BELRA, 	// Dark Belra
    MON_DE_ROL_LE, 		// De Rol Le
    MON_IGNORE, 		// De Rol Le (Shell)
    MON_IGNORE, 		// De Rol Le (Mine)
    MON_DRAGON, 		// Dragon
    MON_SINOW_GOLD, 	// Sinow Gold
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_RAG_RAPPY, 		// Rag Rappy
    MON_AL_RAPPY, 		// Al Rappy
    MON_NANO_DRAGON, 	// Nano Dragon
    MON_DUBCHIC, 		// Dubchic
    MON_GILLCHIC, 		// Gillchic
    MON_GARANZ, 		// Garanz
    MON_DARK_GUNNER, 	// Dark Gunner
    MON_BULCLAW, 		// Bulclaw
    MON_CLAW, 		    // Claw
    MON_IGNORE, 		// Vol Opt (Form 1)
    MON_IGNORE, 		// Vol Opt (Pillar)
    MON_IGNORE, 		// Vol Opt (Monitor)
    MON_IGNORE, 		// Vol Opt (Spire)
    MON_VOL_OPT, 		// Vol Opt (Form 2)
    MON_IGNORE, 		// Vol Opt (Prison)
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_POFUILLY_SLIME, // Pofuilly Slime
    MON_PAN_ARMS, 		// Pan Arms
    MON_HIDOOM, 		// Hidoom
    MON_MIGIUM, 		// Migium
    MON_POUILLY_SLIME, 	// Pouilly Slime
    MON_IGNORE, 		// Darvant
    MON_IGNORE, 		// Dark Falz (Form 1)
    MON_IGNORE, 		// Dark Falz (Form 2)
    MON_DARK_FALZ, 		// Dark Falz (Form 3)
    MON_IGNORE, 		// Darvant (Falz)
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_DUBWITCH, 		// Dubwitch
    MON_HILDEBEAR, 		// Hildebear
    MON_HILDEBLUE, 		// Hildeblue
    MON_BOOMA, 		    // Booma
    MON_GOBOOMA, 		// Gobooma
    MON_GIGOBOOMA, 		// Gigobooma
    MON_GRASS_ASSASSIN, // Grass Assassin
    MON_EVIL_SHARK, 	// Evil Shark
    MON_PAL_SHARK, 		// Pal Shark
    MON_GUIL_SHARK, 	// Guil Shark
    MON_DELSABER, 		// Delsaber
    MON_DIMENIAN, 		// Dimenian
    MON_LA_DIMENIAN, 	// La Dimenian
    MON_SO_DIMENIAN 	// So Dimenian
};

mon_kind ep2_bp_ids[NUM_EP2_BP] = {
    MON_IGNORE, 		// Mothmant
    MON_MONEST, 		// Monest
    MON_SAVAGE_WOLF, 	// Savage Wolf
    MON_BARBAROUS_WOLF, // Barbarous Wolf
    MON_POISON_LILY, 	// Poison Lily
    MON_NAR_LILY, 		// Nar Lily
    MON_SINOW_BERILL, 	// Sinow Berill
    MON_GEE, 			// Gee
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_CHAOS_SORCERER, // Chaos Sorcerer
    MON_IGNORE, 		// Bee R
    MON_IGNORE, 		// Bee L
    MON_DELBITER, 		// Delbiter
    MON_DARK_BELRA, 	// Dark Belra
    MON_BARBA_RAY, 		// Barba Ray
    MON_IGNORE, 		// Pig Ray
    MON_IGNORE, 		// Ul Ray
    MON_GOL_DRAGON, 	// Gol Dragon
    MON_SINOW_SPIGELL, 	// Sinow Spigell
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_RAG_RAPPY, 		// Rag Rappy
    MON_LOVE_RAPPY, 	// Love Rappy
    MON_GI_GUE, 		// Gi Gue
    MON_DUBCHIC, 		// Dubchic
    MON_GILLCHIC, 		// Gillchic
    MON_GARANZ, 		// Garanz
    MON_GAL_GRYPHON, 	// Gal Gryphon
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_EPSILON, 		// Epsilon
    MON_IGNORE, 		// Epsigard
    MON_DEL_LILY, 		// Del Lily
    MON_ILL_GILL, 		// Ill Gill
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// Olga Flow (Form 1)
    MON_OLGA_FLOW, 		// Olga Flow (Form 2)
    MON_IGNORE, 		// Gael
    MON_IGNORE, 		// Giel
    MON_IGNORE, 		// None
    MON_DELDEPTH, 		// Deldepth
    MON_PAN_ARMS, 		// Pan Arms
    MON_HIDOOM, 		// Hidoom
    MON_MIGIUM, 		// Migium
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_MERICAROL, 		// Mericarol
    MON_UL_GIBBON, 		// Ul Gibbon
    MON_ZOL_GIBBON, 	// Zol Gibbon
    MON_GIBBLES, 		// Gibbles
    MON_IGNORE, 		// None
    MON_IGNORE, 		// None
    MON_MORFOS, 		// Morfos
    MON_RECOBOX, 		// Recobox
    MON_RECON, 			// Recon
    MON_SINOW_ZOA, 		// Sinow Zoa
    MON_SINOW_ZELE, 	// Sinow Zele
    MON_MERIKLE, 		// Merikle
    MON_MERICUS, 		// Mericus
    MON_IGNORE, 		// None
    MON_DUBWITCH, 		// Dubwitch
    MON_HILDEBEAR, 		// Hildebear
    MON_HILDEBLUE, 		// Hildeblue
    MON_MERILLIA, 		// Merillia
    MON_MERILTAS, 		// Meriltas
    MON_IGNORE, 		// None
    MON_GRASS_ASSASSIN, // Grass Assassin
    MON_DOLMOLM, 		// Dolmolm
    MON_DOLMDARL, 		// Dolmdarl
    MON_IGNORE, 		// None
    MON_DELSABER, 		// Delsaber
    MON_DIMENIAN, 		// Dimenian
    MON_LA_DIMENIAN, 	// La Dimenian
    MON_SO_DIMENIAN 	// So Dimenian
};

#define NUM_EP4_BP 44
mon_kind ep4_bp_ids[NUM_EP4_BP] = {
    MON_BOOTA, 				// Boota
    MON_ZE_BOOTA, 			// Ze Boota
    MON_IGNORE, 			// None
    MON_BA_BOOTA, 			// Ba Boota
    MON_IGNORE, 			// None
    MON_SAND_RAPPY, 		// Sand Rappy (Crater)
    MON_DEL_RAPPY, 			// Del Rappy (Crater)
    MON_ZU, 				// Zu (Crater)
    MON_PAZUZU, 			// Pazuzu (Crater)
    MON_ASTARK, 			// Astark
    MON_IGNORE, 			// None
    MON_IGNORE, 			// None
    MON_IGNORE, 			// None
    MON_SATELLITE_LIZARD, 	// Satellite Lizard (Crater)
    MON_YOWIE, 				// Yowie (Crater)
    MON_DORPHON, 			// Dorphon
    MON_DORPHON_ECLAIR, 	// Dorphon Eclair
    MON_GORAN, 				// Goran
    MON_PYRO_GORAN, 		// Pyro Goran
    MON_GORAN_DETONATOR, 	// Goran Detonator
    MON_IGNORE, 			// None
    MON_IGNORE, 			// None
    MON_IGNORE, 			// None
    MON_SAND_RAPPY, 		// Sand Rappy (Desert)
    MON_DEL_RAPPY, 			// Del Rappy (Desert)
    MON_MERISSA_A, 			// Merissa A
    MON_MERISSA_AA, 		// Merissa AA
    MON_ZU, 				// Zu (Desert)
    MON_PAZUZU, 			// Pazuzu (Desert)
    MON_SATELLITE_LIZARD, 	// Satellite Lizard (Desert)
    MON_YOWIE, 				// Yowie (Desert)
    MON_GIRTABLULU, 		// Girtablulu
    MON_IGNORE, 			// Saint-Milion (Phase 1)
    MON_IGNORE, 			// Spinner (Saint-Milion 1)
    MON_SAINT_MILION, 		// Saint-Milion (Phase 2)
    MON_IGNORE, 			// Spinner (Saint-Milion 2)
    MON_IGNORE, 			// Shambertin (Phase 1)
    MON_IGNORE, 			// Spinner (Shambertin 1)
    MON_SHAMBERTIN, 		// Shambertin (Phase 2)
    MON_IGNORE, 			// Spinner (Shambertin 2)
    MON_IGNORE, 			// Kondrieu (Phase 1)
    MON_IGNORE, 			// Spinner (Kondrieu 1)
    MON_KONDRIEU, 			// Kondrieu (Phase 2)
    MON_IGNORE  			// Spinner (Kondrieu 2)
};
