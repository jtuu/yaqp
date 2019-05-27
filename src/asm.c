#include <stdint.h>

#include "asm.h"

const size_t arg_sizes[NUM_ARG_KINDS] = {
    [T_NONE]     = 0,
    [T_IMED]     = 0,
    [T_ARGS]     = 0,
    [T_PUSH]     = 0,
    [T_VASTART]  = 0,
    [T_VAEND]    = 0,
    [T_DC]       = 0,
    [T_DWORD]    = 4,
    [T_WORD]     = 2,
    [T_BYTE]     = 1,
    [T_FLOAT]    = 4,
    [T_REG]      = 1,
    [T_BREG]     = 1,
    [T_DREG]     = 0, // ?
    [T_FUNC]     = 2,
    [T_FUNC2]    = 2,
    [T_V2]       = 0,
    [T_V3]       = 0,
    [T_V4]       = 0,
    [T_STR]      = VARIABLE_SIZED,
    [T_PFLAG]    = 0,
    [T_SWITCH]   = VARIABLE_SIZED,
    [T_SWITCH2B] = VARIABLE_SIZED,
    [T_DATA]     = 2,
    [T_STRDATA]  = 0
};

const instruction_t instructions[NUM_INSTRUCTIONS] = {
    {0x00, "nop", {T_NONE, T_NONE}},
    {0x01, "ret", {T_NONE, T_NONE}},
    {0x02, "sync", {T_NONE, T_NONE}},
    {0x03, "exit", {T_ARGS, T_DWORD, T_NONE}},
    {0x04, "thread", {T_IMED, T_FUNC, T_NONE}},
    {0x05, "va_start", {T_VASTART, T_NONE, T_V3}},
    {0x06, "va_end", {T_VAEND, T_NONE, T_V3}},
    {0x07, "va_call", {T_IMED, T_FUNC, T_NONE, T_V3}},
    {0x08, "let", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x09, "leti", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x0A, "letb", {T_IMED, T_REG, T_BYTE, T_NONE}},
    {0x0B, "letw", {T_IMED, T_REG, T_WORD, T_NONE}},
    {0x0C, "leta", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x0D, "leto", {T_IMED, T_REG, T_FUNC, T_NONE}},
    {0x10, "set", {T_IMED, T_REG, T_NONE}},
    {0x11, "clear", {T_IMED, T_REG, T_NONE}},
    {0x12, "rev", {T_IMED, T_REG, T_NONE}},
    {0x13, "gset", {T_IMED, T_PFLAG, T_NONE}},
    {0x14, "gclear", {T_IMED, T_PFLAG, T_NONE}},
    {0x15, "grev", {T_IMED, T_PFLAG, T_NONE}},
    {0x16, "glet", {T_IMED, T_PFLAG, T_NONE}},
    {0x17, "gget", {T_IMED, T_PFLAG, T_REG, T_NONE}},
    {0x18, "add", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x19, "addi", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x1a, "sub", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x1b, "subi", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x1c, "mul", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x1d, "muli", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x1e, "div", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x1f, "divi", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x20, "and", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x21, "andi", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x22, "or", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x23, "ori", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x24, "xor", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x25, "xori", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x26, "mod", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x27, "modi", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x28, "jmp", {T_IMED, T_FUNC, T_NONE}},
    {0x29, "call", {T_IMED, T_FUNC, T_NONE}},
    {0x2a, "jmp_on", {T_IMED, T_FUNC, T_SWITCH2B, T_NONE}},
    {0x2b, "jmp_off", {T_IMED, T_FUNC, T_SWITCH2B, T_NONE}},
    {0x2c, "jmp_=", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x2d, "jmpi_=", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x2e, "jmp_!=", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x2f, "jmpi_!=", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x30, "ujmp_>", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x31, "ujmpi_>", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x32, "jmp_>", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x33, "jmpi_>", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x34, "ujmp_<", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x35, "ujmpi_<", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x36, "jmp_<", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x37, "jmpi_<", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x38, "ujmp_>=", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x39, "ujmpi_>=", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x3a, "jmp_>=", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x3b, "jmpi_>=", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x3c, "ujmp_<=", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x3d, "ujmpi_<=", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x3e, "jmp_<=", {T_IMED, T_REG, T_REG, T_FUNC, T_NONE}},
    {0x3f, "jmpi_<=", {T_IMED, T_REG, T_DWORD, T_FUNC, T_NONE}},
    {0x40, "switch_jmp", {T_IMED, T_REG, T_SWITCH, T_NONE}},
    {0x41, "switch_call", {T_IMED, T_REG, T_SWITCH, T_NONE}},
    {0x42, "stack_push", {T_IMED, T_REG, T_NONE}},
    {0x43, "stack_pop", {T_IMED, T_REG, T_NONE}},
    {0x44, "stack_pushm", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x45, "stack_popm", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0x48, "arg_pushr", {T_PUSH, T_REG, T_NONE, T_V3}},
    {0x49, "arg_pushl", {T_PUSH, T_DWORD, T_NONE, T_V3}},
    {0x4a, "arg_pushb", {T_PUSH, T_BREG, T_NONE, T_V3}},
    {0x4b, "arg_pushw", {T_PUSH, T_WORD, T_NONE, T_V3}},
    {0x4C, "arg_pusha", {T_PUSH, T_REG, T_NONE, T_V3}},
    {0x4D, "arg_pusho", {T_PUSH, T_FUNC, T_NONE, T_V3}},
    {0x4e, "arg_pushs", {T_PUSH, T_STR, T_NONE, T_V3}},
    {0x50, "message", {T_ARGS, T_DWORD, T_STR, T_NONE}},
    {0x51, "list", {T_ARGS, T_BREG, T_STR, T_NONE}},
    {0x52, "fadein", {T_NONE, T_NONE}},
    {0x53, "fadeout", {T_NONE, T_NONE}},
    {0x54, "se", {T_ARGS, T_DWORD, T_NONE}},
    {0x55, "bgm", {T_ARGS, T_DWORD, T_NONE}},
    {0x58, "enable", {T_ARGS, T_DWORD, T_NONE}},
    {0x59, "disable", {T_ARGS, T_DWORD, T_NONE}},
    {0x5a, "window_msg", {T_ARGS, T_STR, T_NONE}},
    {0x5b, "add_msg", {T_ARGS, T_STR, T_NONE}},
    {0x5c, "mesend", {T_NONE, T_NONE}},
    {0x5d, "gettime", {T_IMED, T_REG, T_NONE}},
    {0x5e, "winend", {T_NONE, T_NONE}},
    // {0x60, "npc_crt_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x60, "npc_crt_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x61, "npc_stop", {T_ARGS, T_DWORD, T_NONE}},
    {0x62, "npc_play", {T_ARGS, T_DWORD, T_NONE}},
    {0x63, "npc_kill", {T_ARGS, T_DWORD, T_NONE}},
    {0x64, "npc_nont", {T_NONE, T_NONE}},
    {0x65, "npc_talk", {T_NONE, T_NONE}},
    // {0x66, "npc_crp_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x66, "npc_crp_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x68, "create_pipe", {T_ARGS, T_DWORD, T_NONE}},
    // {0x69, "p_hpstat_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x69, "p_hpstat_V3", {T_ARGS, T_BREG, T_DWORD, T_NONE, T_V3}},
    // {0x6a, "p_dead_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x6a, "p_dead_V3", {T_ARGS, T_BREG, T_REG, T_NONE, T_V3}},
    {0x6b, "p_disablewarp", {T_NONE, T_NONE}},
    {0x6c, "p_enablewarp", {T_NONE, T_NONE}},
    // {0x6d, "p_move_v1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x6d, "p_move_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x6e, "p_look", {T_ARGS, T_DWORD, T_NONE}},
    {0x70, "p_action_disable", {T_NONE, T_NONE}},
    {0x71, "p_action_enable", {T_NONE, T_NONE}},
    {0x72, "disable_movement1", {T_ARGS, T_DWORD, T_NONE}},
    {0x73, "enable_movement1", {T_ARGS, T_DWORD, T_NONE}},
    {0x74, "p_noncol", {T_NONE, T_NONE}},
    {0x75, "p_col", {T_NONE, T_NONE}},
    {0x76, "p_setpos", {T_ARGS, T_DWORD, T_BREG, T_NONE}},
    {0x77, "p_return_guild", {T_NONE, T_NONE}},
    {0x78, "p_talk_guild", {T_ARGS, T_DWORD, T_NONE}},
    // {0x79, "npc_talk_pl_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x79, "npc_talk_pl_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x7a, "npc_talk_kill", {T_ARGS, T_DWORD, T_NONE}},
    // {0x7b, "npc_crtpk_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x7b, "npc_crtpk_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    // {0x7c, "npc_crppk_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x7c, "npc_crppk_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    // {0x7d, "npc_crptalk_v1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x7d, "npc_crptalk_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x7e, "p_look_at", {T_ARGS, T_DWORD, T_DWORD, T_NONE}},
    // {0x7f, "npc_crp_id_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x7f, "npc_crp_id_v3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x80, "cam_quake", {T_NONE, T_NONE}},
    {0x81, "cam_adj", {T_NONE, T_NONE}},
    {0x82, "cam_zmin", {T_NONE, T_NONE}},
    {0x83, "cam_zmout", {T_NONE, T_NONE}},
    // {0x84, "cam_pan_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x84, "cam_pan_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x85, "game_lev_super", {T_NONE, T_NONE}},
    {0x86, "game_lev_reset", {T_NONE, T_NONE}},
    // {0x87, "pos_pipe_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0x87, "pos_pipe_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0x88, "if_zone_clear", {T_IMED, T_REG, T_REG, T_NONE}},
    {0x89, "chk_ene_num", {T_IMED, T_REG, T_NONE}},
    {0x8a, "unhide_obj", {T_IMED, T_REG, T_NONE}},
    {0x8b, "unhide_ene", {T_IMED, T_REG, T_NONE}},
    {0x8c, "at_coords_call", {T_IMED, T_REG, T_NONE}},
    {0x8d, "at_coords_talk", {T_IMED, T_REG, T_NONE}},
    {0x8e, "col_npcin", {T_IMED, T_REG, T_NONE}},
    {0x8f, "col_npcinr", {T_IMED, T_REG, T_NONE}},
    {0x90, "switch_on", {T_ARGS, T_DWORD, T_NONE}},
    {0x91, "switch_off", {T_ARGS, T_DWORD, T_NONE}},
    {0x92, "playbgm_epi", {T_ARGS, T_DWORD, T_NONE}},
    {0x93, "set_mainwarp", {T_ARGS, T_DWORD, T_NONE}},
    {0x94, "set_obj_param", {T_IMED, T_REG, T_BREG, T_NONE}},
    {0x95, "set_floor_handler", {T_ARGS, T_DWORD, T_FUNC2, T_NONE}},
    {0x96, "clr_floor_handler", {T_ARGS, T_DWORD, T_NONE}},
    {0x97, "col_plinaw", {T_IMED, T_REG, T_NONE}},
    {0x98, "hud_hide", {T_NONE, T_NONE}},
    {0x99, "hud_show", {T_NONE, T_NONE}},
    {0x9a, "cine_enable", {T_NONE, T_NONE}},
    {0x9b, "cine_disable", {T_NONE, T_NONE}},
    {0xa1, "set_qt_failure", {T_IMED, T_FUNC2, T_NONE}},
    {0xa2, "set_qt_success", {T_IMED, T_FUNC2, T_NONE}},
    {0xa3, "clr_qt_failure", {T_NONE, T_NONE}},
    {0xa4, "clr_qt_success", {T_NONE, T_NONE}},
    {0xa5, "set_qt_cancel", {T_IMED, T_FUNC2, T_NONE}},
    {0xa6, "clr_qt_cancel", {T_NONE, T_NONE}},
    // {0xa8, "pl_walk_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xa8, "pl_walk_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xb0, "pl_add_meseta", {T_ARGS, T_DWORD, T_DWORD, T_NONE}},
    {0xb1, "thread_stg", {T_IMED, T_FUNC, T_NONE}},
    {0xb2, "del_obj_param", {T_IMED, T_REG, T_NONE}},
    {0xb3, "item_create", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xb4, "item_create2", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xb5, "item_delete", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xb6, "item_delete2", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xb7, "item_check", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xb8, "setevt", {T_ARGS, T_DWORD, T_NONE}},
    {0xb9, "get_difflvl", {T_IMED, T_REG, T_NONE}},
    {0xba, "set_qt_exit", {T_IMED, T_FUNC2, T_NONE}},
    {0xbb, "clr_qt_exit", {T_NONE, T_NONE}},
    // {0xc0, "particle_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xc0, "particle_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xc1, "npc_text", {T_ARGS, T_DWORD, T_STR, T_NONE}},
    {0xc2, "npc_chkwarp", {T_NONE, T_NONE}},
    {0xc3, "pl_pkoff", {T_NONE, T_NONE}},
    {0xc4, "map_designate", {T_IMED, T_REG, T_NONE}},
    {0xc5, "masterkey_on", {T_NONE, T_NONE}},
    {0xc6, "masterkey_off", {T_NONE, T_NONE}},
    {0xc7, "window_time", {T_NONE, T_NONE}},
    {0xc8, "winend_time", {T_NONE, T_NONE}},
    {0xc9, "winset_time", {T_IMED, T_REG, T_NONE}},
    {0xca, "getmtime", {T_IMED, T_REG, T_NONE}},
    {0xcb, "set_quest_board_handler", {T_ARGS, T_DWORD, T_FUNC2, T_STR, T_NONE}},
    {0xcc, "clear_quest_board_handler", {T_ARGS, T_DWORD, T_NONE}},
    // {0xcd, "particle_id_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xcd, "particle_id_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    // {0xce, "npc_crptalk_id_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xce, "npc_crptalk_id_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xcf, "npc_lang_clean", {T_NONE, T_NONE}},
    {0xd0, "pl_pkon", {T_NONE, T_NONE}},
    {0xd1, "pl_chk_item2", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xd2, "enable_mainmenu", {T_NONE, T_NONE}},
    {0xd3, "disable_mainmenu", {T_NONE, T_NONE}},
    {0xd4, "start_battlebgm", {T_NONE, T_NONE}},
    {0xd5, "end_battlebgm", {T_NONE, T_NONE}},
    {0xd6, "disp_msg_qb", {T_ARGS, T_STR, T_NONE}},
    {0xd7, "close_msg_qb", {T_NONE, T_NONE}},
    // {0xd8, "set_eventflag_v1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xd8, "set_eventflag_v3", {T_ARGS, T_DWORD, T_DWORD, T_NONE}},
    {0xd9, "sync_leti", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xda, "set_returnhunter", {T_NONE, T_NONE}},
    {0xdb, "set_returncity", {T_NONE, T_NONE}},
    {0xdc, "load_pvr", {T_NONE, T_NONE}},
    {0xdd, "load_midi", {T_NONE, T_NONE}},
    {0xde, "unknownDE", {T_IMED, T_REG, T_NONE}},
    // {0xdf, "npc_param_V1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xdf, "npc_param_V3", {T_ARGS, T_BREG, T_DWORD, T_NONE, T_V3}},
    {0xe0, "pad_dragon", {T_NONE, T_NONE}},
    {0xe1, "clear_mainwarp", {T_ARGS, T_DWORD, T_NONE}},
    // {0xe2, "pcam_param_V1", {T_DC, T_DREG, T_NONE}},
    {0xe2, "pcam_param_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    // {0xe3, "start_setevt_v1", {T_DC, T_DREG, T_DWORD, T_NONE}},
    {0xe3, "start_setevt_v3", {T_ARGS, T_BREG, T_DWORD, T_NONE}},
    {0xe4, "warp_on", {T_NONE, T_NONE}},
    {0xe5, "warp_off", {T_NONE, T_NONE}},
    {0xe6, "get_slotnumber", {T_IMED, T_REG, T_NONE}},
    {0xe7, "get_servernumber", {T_IMED, T_REG, T_NONE}},
    {0xe8, "set_eventflag2", {T_ARGS, T_DWORD, T_REG, T_NONE}},
    {0xe9, "res", {T_IMED, T_REG, T_REG, T_NONE}},
    {0xea, "unknownEA", {T_IMED, T_REG, T_DWORD, T_NONE}},
    {0xeb, "enable_bgmctrl", {T_ARGS, T_DWORD, T_NONE}},
    {0xec, "sw_send", {T_IMED, T_REG, T_NONE}},
    {0xed, "create_bgmctrl", {T_NONE, T_NONE}},
    {0xee, "pl_add_meseta2", {T_ARGS, T_DWORD, T_NONE}},
    // {0xef, "sync_let", {T_DC, T_DREG, T_DREG, T_NONE}},
    {0xef, "sync_register", {T_ARGS, T_BREG, T_DWORD, T_NONE, T_V3}},
    {0xef, "sync_register2", {T_ARGS, T_BREG, T_REG, T_NONE, T_V3}},
    {0xf0, "send_regwork", {T_DC, T_DREG, T_DWORD, T_NONE}},
    // {0xf1, "leti_fixed_camera_V1", {T_DC, T_DREG, T_NONE}},
    {0xf1, "leti_fixed_camera_V3", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf2, "default_camera_pos1", {T_NONE, T_NONE}},
    {0xf8, "unknownF8", {T_IMED, T_REG, T_NONE}},
    {0xfa, "get_gc_number", {T_IMED, T_REG, T_NONE}},
    {0xfb, "unknownFB", {T_IMED, T_DATA, T_NONE}},
    {0xf801, "set_chat_callback?", {T_ARGS, T_BREG, T_STR, T_NONE, T_V2}},
    {0xf808, "get_difflvl2", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf809, "get_number_of_player1", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf80a, "get_coord_of_player", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf80b, "enable_map", {T_NONE, T_NONE, T_V2}},
    {0xf80c, "disable_map", {T_NONE, T_NONE, T_V2}},
    {0xf80d, "map_designate_ex", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf80e, "unknownF80E", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf80f, "unknownF80F", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf810, "ba_initial_floor", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf811, "set_ba_rules", {T_NONE, T_NONE, T_V2}},
    {0xf812, "unknownF812", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf813, "unknownF813", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf814, "unknownF814", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf815, "unknownF815", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf816, "unknownF816", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf817, "unknownF817", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf818, "unknownF818", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf819, "unknownF819", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf81a, "unknownF81A", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf81b, "unknownF81B", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf81c, "ba_disp_msg", {T_ARGS, T_STR, T_NONE, T_V2}},
    {0xf81d, "death_lvl_up", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf81e, "death_tech_lvl_up", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf820, "cmode_stage", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf823, "unknownF823", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf824, "unknownF824", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf825, "exp_multiplication", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf826, "exp_division?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf827, "get_user_is_dead?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf828, "go_floor", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf82b, "unlock_door2", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V2}},
    {0xf82c, "lock_door2", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V2}},
    {0xf82d, "if_switch_not_pressed", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf82e, "if_switch_pressed", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf82f, "unknownF82F", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V2}},
    {0xf830, "control_dragon", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf831, "release_dragon", {T_NONE, T_NONE, T_V2}},
    {0xf838, "shrink", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf839, "unshrink", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf83c, "display_clock2?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf83d, "unknownF83D", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf83e, "delete_area_title?", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf840, "load_npc_data", {T_NONE, T_NONE, T_V2}},
    {0xf841, "get_npc_data", {T_IMED, T_DATA, T_NONE, T_V2}},
    {0xf848, "give_damage_score", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf849, "take_damage_score", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf84a, "unk_score_F84A", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf84b, "unk_score_F84B", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf84c, "kill_score", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf84d, "death_score", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf84e, "unk_score_F84E", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf84f, "enemy_death_score", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf850, "meseta_score", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf851, "unknownF851", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf852, "unknownF852", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf853, "reverse_warps", {T_NONE, T_NONE, T_V2}},
    {0xf854, "unreverse_warps", {T_NONE, T_NONE, T_V2}},
    {0xf855, "set_ult_map", {T_NONE, T_NONE, T_V2}},
    {0xf856, "unset_ult_map", {T_NONE, T_NONE, T_V2}},
    {0xf857, "set_area_title", {T_ARGS, T_STR, T_NONE, T_V2}},
    {0xf858, "unknownF858", {T_NONE, T_NONE, T_V2}},
    // {0xf85a, "equip_item_v2", {T_DC, T_DREG, T_NONE, T_V2}},
    {0xf85a, "equip_item_v3", {T_IMED, T_REG, T_NONE, T_V3}},
    // {0xf85b, "unequip_item_V2", {T_DC, T_DREG, T_DWORD, T_NONE, T_V2}},
    {0xf85b, "unequip_item_V3", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V3}},
    {0xf858, "BA_Show_Self_Traps", {T_NONE}},
    {0xf859, "BA_Hide_Self_Traps", {T_NONE}},
    {0xf85e, "unknownF85E", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf85f, "unknownF85F", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf860, "unknownF860", {T_NONE, T_NONE, T_V2}},
    {0xf861, "unknownF861", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf864, "cmode_rank", {T_ARGS, T_DWORD, T_STR, T_NONE, T_V2}},
    {0xf865, "award_item_name?", {T_NONE, T_NONE, T_V2}},
    {0xf866, "award_item_select?", {T_NONE, T_NONE, T_V2}},
    {0xf867, "award_item_give_to?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf868, "unknownF868", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf869, "unknownF869", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf86a, "item_create_cmode", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf86b, "unknownF86B", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf86c, "award_item_ok?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf86d, "unknownF86D", {T_NONE, T_NONE, T_V2}},
    {0xf86e, "unknownF86E", {T_NONE, T_NONE, T_V2}},
    {0xf86f, "ba_set_lives", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf870, "ba_set_tech_lvl", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf871, "ba_set_lvl", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf872, "ba_set_time_limit", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf873, "boss_is_dead?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf877, "enable_techs", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf878, "disable_techs", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf879, "get_gender", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf87a, "get_chara_class", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf87b, "take_slot_meseta", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf87f, "read_guildcard_flag", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf880, "unknownF880", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf881, "get_pl_name?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf883, "unknownF883", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf888, "ba_close_msg", {T_NONE, T_NONE, T_V2}},
    {0xf88a, "get_player_status", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf88b, "send_mail", {T_ARGS, T_BREG, T_STR, T_NONE, T_V2}},
    {0xf88c, "get_game_version", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf88d, "chl_set_timerecord?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf88e, "chl_get_timerecord?", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf88f, "unknownF88F", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf890, "unknownF890", {T_NONE, T_NONE, T_V2}},
    {0xf891, "load_enemy_data", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf892, "get_physical_data", {T_IMED, T_DATA, T_NONE, T_V2}},
    {0xf893, "get_attack_data", {T_IMED, T_DATA, T_NONE, T_V2}},
    {0xf894, "get_resist_data", {T_IMED, T_DATA, T_NONE, T_V2}},
    {0xf895, "get_movement_data", {T_IMED, T_DATA, T_NONE, T_V2}},
    {0xf898, "shift_left", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf899, "shift_right", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf89a, "get_random", {T_IMED, T_REG, T_REG, T_NONE, T_V2}},
    {0xf89b, "reset_map", {T_NONE, T_NONE, T_V2}},
    {0xf89c, "disp_chl_retry_mnu", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf89d, "chl_reverser?", {T_NONE, T_NONE, T_V2}},
    {0xf89e, "unknownF89E", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf89f, "unknownF89F", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf8a0, "unknownF8A0", {T_NONE, T_NONE, T_V2}},
    {0xf8a1, "unknownF8A1", {T_NONE, T_NONE, T_V2}},
    {0xf8a8, "unknownF8A8", {T_ARGS, T_DWORD, T_NONE, T_V2}},
    {0xf8a9, "unknownF8A9", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf8ad, "get_number_of_player2", {T_IMED, T_REG, T_NONE, T_V2}},
    {0xf8b8, "unknownF8B8", {T_NONE, T_NONE, T_V2}},
    {0xf8b9, "chl_recovery?", {T_NONE, T_NONE, T_V2}},
    {0xf8bc, "set_episode", {T_IMED, T_DWORD, T_NONE, T_V3}},
    {0xf8c0, "file_dl_req", {T_ARGS, T_DWORD, T_STR, T_NONE, T_V3}},
    {0xf8c1, "get_dl_status", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8c2, "gba_unknown4?", {T_NONE, T_NONE, T_V3}},
    {0xf8c3, "get_gba_state?", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8c4, "unknownF8C4", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8c5, "unknownF8C5", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8c6, "QEXIT", {T_NONE, T_NONE, T_V3}},
    {0xf8c7, "use_animation", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8c8, "stop_animation", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8c9, "run_to_coord", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8ca, "set_slot_invincible", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8cb, "unknownF8CB", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8cc, "set_slot_poison", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8cd, "set_slot_paralyse", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8ce, "set_slot_shock", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8cf, "set_slot_freeze", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d0, "set_slot_slow", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d1, "set_slot_confuse", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d2, "set_slot_shifta", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d3, "set_slot_deband", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d4, "set_slot_jellen", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d5, "set_slot_zalure", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8d6, "fleti_fixed_camera", {T_ARGS, T_BREG, T_NONE, T_V3}},
    {0xf8d7, "fleti_locked_camera", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf8d8, "default_camera_pos2", {T_NONE, T_NONE, T_V3}},
    {0xf8d9, "set_motion_blur", {T_NONE, T_NONE, T_V3}},
    {0xf8da, "set_screen_b&w", { T_NONE, T_NONE, T_V3}},
    {0xf8db, "unknownF8DB", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_BREG, T_FUNC, T_NONE, T_V3}},
    {0xf8dc, "NPC_action_string", {T_IMED, T_REG, T_REG, T_STRDATA, T_NONE, T_V3}},
    {0xf8dd, "get_pad_cond", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8de, "get_button_cond", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8df, "freeze_enemies", {T_NONE, T_NONE, T_V3}},
    {0xf8e0, "unfreeze_enemies", {T_NONE, T_NONE, T_V3}},
    {0xf8e1, "freeze_everything", {T_NONE, T_NONE, T_V3}},
    {0xf8e2, "unfreeze_everything", {T_NONE, T_NONE, T_V3}},
    {0xf8e3, "restore_hp", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8e4, "restore_tp", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8e5, "close_chat_bubble", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf8e6, "unknownF8E6", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8e7, "unknownF8E7", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8e8, "unknownF8E8", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8e9, "unknownF8E9", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8ea, "unknownF8EA", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8eb, "unknownF8EB", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8ec, "unknownF8EC", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8ed, "animation_check", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf8ee, "call_image_data", {T_ARGS, T_DWORD, T_DATA, T_NONE, T_V3}},
    {0xf8ef, "unknownF8EF", {T_NONE, T_NONE, T_V3}},
    {0xf8f0, "turn_off_bgm_p2", {T_NONE, T_NONE, T_V3}},
    {0xf8f1, "turn_on_bgm_p2", {T_NONE, T_NONE, T_V3}},
    {0xf8f2, "load_unk_data", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_BREG, T_DATA, T_NONE, T_V3}},
    {0xf8f3, "particle2", {T_ARGS, T_BREG, T_REG, T_REG, T_NONE, T_V3}},
    {0xf901, "dec2float", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf902, "float2dec", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf903, "flet", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf904, "fleti", {T_IMED, T_REG, T_FLOAT, T_NONE, T_V3}},
    {0xf908, "fadd", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf909, "faddi", {T_IMED, T_REG, T_FLOAT, T_NONE, T_V3}},
    {0xf90a, "fsub", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf90b, "fsubi", {T_IMED, T_REG, T_FLOAT, T_NONE, T_V3}},
    {0xf90c, "fmul", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf90d, "fmuli", {T_IMED, T_REG, T_FLOAT, T_NONE, T_V3}},
    {0xf90e, "fdiv", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf90f, "fdivi", {T_IMED, T_REG, T_FLOAT, T_NONE, T_V3}},
    {0xf910, "get_unknown_count?", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf911, "get_stackable_item_count", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf912, "freeze_and_hide_equip", {T_NONE, T_NONE, T_V3}},
    {0xf913, "thaw_and_show_equip", {T_NONE, T_NONE, T_V3}},
    {0xf914, "set_paletteX_callback", {T_ARGS, T_REG, T_FUNC2, T_NONE, T_V3}},
    {0xf915, "activate_paletteX", {T_ARGS, T_REG, T_NONE, T_V3}},
    {0xf916, "enable_paletteX", {T_ARGS, T_REG, T_NONE, T_V3}},
    {0xf917, "restore_paletteX", {T_ARGS, T_DWORD, T_NONE, T_V3}},
    {0xf918, "disable_paletteX", {T_ARGS, T_DWORD, T_NONE, T_V3}},
    {0xf919, "get_paletteX_activated", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf91a, "get_unknown_paletteX_status?", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf91b, "disable_movement2", {T_ARGS, T_REG, T_NONE, T_V3}},
    {0xf91c, "enable_movement2", {T_ARGS, T_REG, T_NONE, T_V3}},
    {0xf91d, "get_time_played", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf91e, "get_guildcard_total", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf91f, "get_slot_meseta", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf920, "get_player_level", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf921, "get_Section_ID", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf922, "get_player_hp", {T_ARGS, T_REG, T_BREG, T_NONE, T_V3}},
    {0xf923, "get_floor_number", {T_ARGS, T_REG, T_BREG, T_NONE, T_V3}},
    {0xf924, "get_coord_player_detect", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf925, "read_global_flag", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf926, "write_global_flag", {T_ARGS, T_DWORD, T_REG, T_NONE, T_V3}},
    {0xf927, "unknownF927", {T_IMED, T_REG, T_REG, T_NONE, T_V3}},
    {0xf928, "floor_player_detect", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf929, "read_disk_file?", {T_ARGS, T_STR, T_NONE, T_V3}},
    {0xf92a, "open_pack_select", {T_NONE, T_NONE, T_V3}},
    {0xf92b, "item_select", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf92c, "get_item_id", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf92d, "color_change", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_NONE, T_V3}},
    {0xf92e, "send_statistic?", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_NONE, T_V3}},
    {0xf92f, "unknownF92F", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V3}},
    {0xf930, "chat_box", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_STR, T_NONE, T_V3}},
    {0xf931, "chat_bubble", {T_ARGS, T_DWORD, T_STR, T_NONE, T_V3}},
    {0xf933, "unknownF933", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf934, "scroll_text", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_FLOAT, T_BREG, T_STR, T_NONE, T_V3}},
    {0xf935, "gba_unknown1", {T_NONE, T_NONE, T_V3}},
    {0xf936, "gba_unknown2", {T_NONE, T_NONE, T_V3}},
    {0xf937, "gba_unknown3", {T_NONE, T_NONE, T_V3}},
    {0xf938, "add_damage_to?", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V3}},
    {0xf939, "item_delete2", {T_ARGS, T_DWORD, T_NONE, T_V3}},
    {0xf93a, "get_item_info", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf93b, "item_packing1", {T_ARGS, T_DWORD, T_NONE, T_V3}},
    {0xf93c, "item_packing2", {T_ARGS, T_DWORD, T_DWORD, T_NONE, T_V3}},
    {0xf93d, "get_lang_setting?", {T_ARGS, T_BREG, T_NONE, T_V3}},
    {0xf93e, "prepare_statistic?", {T_ARGS, T_DWORD, T_FUNC, T_FUNC, T_NONE, T_V3}},
    {0xf93f, "keyword_detect", {T_NONE, T_NONE, T_V3}},
    {0xf940, "Keyword", {T_ARGS, T_BREG, T_DWORD, T_STR, T_NONE, T_V3}},
    {0xf941, "get_guildcard_num", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf944, "get_wrap_status", {T_ARGS, T_DWORD, T_BREG, T_NONE, T_V3}},
    {0xf945, "initial_floor", {T_ARGS, T_DWORD, T_NONE, T_V3}},
    {0xf946, "sin", {T_ARGS, T_BREG, T_DWORD, T_NONE, T_V3}},
    {0xf947, "cos", {T_ARGS, T_BREG, T_DWORD, T_NONE, T_V3}},
    {0xf94a, "boss_is_dead2?", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf94b, "unknownF94B", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf94c, "unknownF94C", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf94d, "is_there_cardbattle?", {T_IMED, T_REG, T_NONE, T_V3}},
    {0xf950, "BB_p2_menu", {T_ARGS, T_DWORD, T_NONE, T_V4}},
    {0xf951, "BB_Map_Designate", {T_IMED, T_BYTE, T_WORD, T_BYTE, T_BYTE, T_NONE, T_V4}},
    {0xf952, "BB_get_number_in_pack", {T_IMED, T_REG, T_NONE, T_V4}},
    {0xf953, "BB_swap_item", {T_ARGS, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_DWORD, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf954, "BB_check_wrap", {T_ARGS, T_REG, T_BREG, T_NONE, T_V4}},
    {0xf955, "BB_exchange_PD_item", {T_ARGS, T_REG, T_REG, T_REG, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf956, "BB_exchange_PD_srank", {T_ARGS, T_REG, T_REG, T_REG, T_REG, T_REG, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf957, "BB_exchange_PD_special", {T_ARGS, T_REG, T_REG, T_REG, T_REG, T_REG, T_DWORD, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf958, "BB_exchange_PD_percent", {T_ARGS, T_REG, T_REG, T_REG, T_REG, T_REG, T_DWORD, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf959, "unknownF959", {T_ARGS, T_DWORD, T_NONE, T_V4}},
    {0xf95c, "BB_exchange_SLT", {T_ARGS, T_DWORD, T_BREG, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf95d, "BB_exchange_PC", {T_NONE, T_NONE, T_V4}},
    {0xf95e, "BB_box_create_BP", {T_ARGS, T_DWORD, T_FLOAT, T_FLOAT, T_NONE, T_V4}},
    {0xf95f, "BB_exchage_PT", {T_ARGS, T_BREG, T_BREG, T_DWORD, T_FUNC2, T_FUNC2, T_NONE, T_V4}},
    {0xf960, "unknownF960", {T_ARGS, T_REG, T_NONE, T_V4}},
    {0xf961, "unknownF961", {T_IMED, T_REG, T_NONE, T_V4}}
};
