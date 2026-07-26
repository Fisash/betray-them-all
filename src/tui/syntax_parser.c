#include "cli/syntax_parser.h"

static int match_string(const char *s, const char *strings[])
{
    int i = 0;
    while(strings[i] != NULL)
    {
        if(strcmp(s, strings[i]) == 0)
            return 1;
        i++;
    }
    return 0;
}

static enum command_type define_command_type(const char* first_word, 
                                   const struct command_info info[])
{
    enum command_type type;
    for(type = 0; type < CMD_COUNT; type++)
        if(match_string(first_word, info[type].cmd_synonyms))
            return type; 
    return CMD_UNKNOWN;
      
}

static void parse_move_args(struct parsed_command *out,
                         const struct command *lex_cmd)
{
    switch(lex_cmd->argv[1])
    {
        case 'r':
            out->args.move.order = MOVE_RIGHT;
            break;
        case 'l':
            out->args.move.order = MOVE_LEFT;
            break;
        case 'u':
            out->args.move.order = MOVE_UP;
            break;
        case 'd':
            out->args.move.order = MOVE_DOWN;
            break;
        case 's':
            out->args.move.order = MOVE_NONE;
            break;
        default:
            return;
    }

    out->is_correct = 1;
}


static void parse_info_unit_args(struct parsed_command *out,
                              const struct command *lex_cmd)
{
    uint16_t unit_num = atoi(lex_cmd->argv[2]);
    if(!unit_num)
        return;

    out->args.info_unit.unit_num = unit_num;
    out->is_correct = 1;
}

static void parse_info_item_args(struct parsed_command *out,
                              const struct command *lex_cmd)
{
    uint16_t item_num = atoi(lex_cmd->argv[2]);
    if(!item_num)
        return;

    out->args.info_item.item_num = item_num;
    out->is_correct = 1;
}

static void parse_equip_args(struct parsed_command *out,
                          const struct command *lex_cmd)
{
    uint16_t unit_num = atoi(lex_cmd->argv[1]);
    uint16_t item_num = atoi(lex_cmd->argv[2]);
    if(!unit_num || !item_num)
        return;
    out->args.equip.unit_num = unit_num;
    out->args.equip.item_num = item_num;
    out->is_correct = 1;
}

static void parse_unequip_args(struct parsed_command *out,
                            const struct command *lex_cmd)
{

    uint16_t unit_num;
    enum item_type equipment_type = ITEM_TYPE_GENERIC;

    unit_num = atoi(lex_cmd->argv[1]);
    if(!unit_num)
        return;

    if(strcmp(lex_cmd->argv[2], "weapon") == 0)
        equipment_type = ITEM_TYPE_WEAPON;
    else if(strcmp(lex_cmd->argv[2], "armor") == 0)
        equipment_type = ITEM_TYPE_ARMOR;
    else 
        return;

    out->args.unequip.unit_num = unit_num;
    out->args.unequip.equipment_type = equipment_type;
    out->is_correct = 1;
}

static void parse_improve_args(struct parsed_command *out,
                            const struct command *lex_cmd)
{
    enum stat_selection stat;
    uint16_t unit_num = atoi(lex_cmd->argv[1]);
    if(!unit_num)
        return;

    if(strcmp(cmd->argv[2], "str") == 0)
        stat = STRENGTH;
    else if(strcmp(cmd->argv[2], "agl") == 0)
        stat = AGILITY;
    else if(strcmp(cmd->argv[2], "wil") == 0)
        stat = WILL;
    else if(strcmp(cmd->argv[2], "int") == 0)
        stat = INTELLIGENCE;
    else
        return;

    out->args.improve.unit_num = unit_num;
    out->args.improve.stat = stat;
    out->is_correct = 1;
}
                    
void syntax_command_parse(struct parsed_command *out, 
                       const struct command *lex_cmd,
                    const struct command_info info[])
{

    char *first_word = lex_cmd->argv[0];
    if(!first_word || strlen(first_word) == 0)
       return;

    out->type = define_command_type(first_word, info); 
    if(lex_cmd->agrc < info[out->type].min_argc+1)
        return;

    switch(out->type)
    {
        case CMD_MOVE:
            parse_move_args(out, lex_cmd); 
            break;
        case CMD_INFO_UNIT:
            parse_info_unit_args(out, lex_cmd);
            break;
        case CMD_INFO_ITEM:
            parse_info_item_args(out, lex_cmd);
            break;
        case CMD_EQUIP:
            parse_equip_args(out, lex_cmd);
            break;
        case CMD_UNEQUIP:
            parse_unequip_args(out, lex_cmd);
            break;
        case CMD_IMPROVE:
            parse_improve_args(out, lex_cmd);
            break;
        default:
            is_correct = 1;
            break;
    }
}

void syntax_command_input(struct parsed_command *cmd, char input_buffer[],
                                         const struct command_info info[])
{
    struct command lex_cmd;
    lexical_command_input(&lex_cmd, input_buffer);
    syntax_command_parse(cmd, &lex_cmd, info);
}

