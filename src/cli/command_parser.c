#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "cli/command_parser.h"

#define ARGS_SPLITER " \n"

void command_parse_lexical(char *input_buf, struct command *out)
{
    out->argc = 0;
    char* current_token = strtok(input_buf, ARGS_SPLITER);
    while (current_token != NULL && out->argc < ARGC_MAX)
    {
        out->argv[out->argc] = current_token;
        out->argc++;
        current_token = strtok(NULL, ARGS_SPLITER);
    }
}

static int match_string(const char *s, const char * const strings[])
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
                                  const struct command_info *info[])
{
    enum command_type type;
    for(type = 0; type < CMD_COUNT; type++)
        if(match_string(first_word, info[type]->synonyms))
            return type; 
    return CMD_UNKNOWN;
      
}

static void parse_move_args(struct parsed_command *out,
                         const struct command *lex_cmd)
{
    switch(lex_cmd->argv[1][0])
    {
        case 'r':
            out->args.move_order = MOVE_RIGHT;
            break;
        case 'l':
            out->args.move_order = MOVE_LEFT;
            break;
        case 'u':
            out->args.move_order = MOVE_UP;
            break;
        case 'd':
            out->args.move_order = MOVE_DOWN;
            break;
        case 's':
            out->args.move_order = MOVE_NONE;
            break;
        default:
            return;
    }

    out->is_correct = 1;
}

static void parse_num(int *result_status, const char *source,
                                         uint16_t *recipient)
{
    uint16_t value = atoi(source);
    *result_status = (value > 0);
    if(*result_status)
        *recipient = value;
}

static void parse_equip_args(struct parsed_command *out,
                          const struct command *lex_cmd)
{
    int is_unit_parsed, is_item_parsed;
    parse_num(&is_unit_parsed, lex_cmd->argv[1], 
                     &out->args.equip.unit_num);
    parse_num(&is_item_parsed, lex_cmd->argv[2], 
                     &out->args.equip.item_num);

    if(is_unit_parsed && is_item_parsed)
        out->is_correct = 1;
}

static void parse_unequip_args(struct parsed_command *out,
                            const struct command *lex_cmd)
{

    int is_unit_parsed;
    enum item_type equipment_type = ITEM_TYPE_GENERIC;

    parse_num(&is_unit_parsed, lex_cmd->argv[1], 
                   &out->args.unequip.unit_num);
    if(!is_unit_parsed)
        return;

    if(strcmp(lex_cmd->argv[2], "weapon") == 0)
        equipment_type = ITEM_TYPE_WEAPON;
    else if(strcmp(lex_cmd->argv[2], "armor") == 0)
        equipment_type = ITEM_TYPE_ARMOR;
    else 
        return;

    out->args.unequip.equipment_type = equipment_type;
    out->is_correct = 1;
}

static void parse_improve_args(struct parsed_command *out,
                            const struct command *lex_cmd)
{
    int is_unit_parsed;
    enum stat_selection stat;

    parse_num(&is_unit_parsed, lex_cmd->argv[1], 
                   &out->args.improve.unit_num);
    if(!is_unit_parsed)
        return;

    if(strcmp(lex_cmd->argv[2], "str") == 0)
        stat = STRENGTH;
    else if(strcmp(lex_cmd->argv[2], "agl") == 0)
        stat = AGILITY;
    else if(strcmp(lex_cmd->argv[2], "wil") == 0)
        stat = WILL;
    else if(strcmp(lex_cmd->argv[2], "int") == 0)
        stat = INTELLIGENCE;
    else
        return;

    out->args.improve.stat = stat;
    out->is_correct = 1;
}
                    
void command_parse_syntax(struct parsed_command *out, 
                          const struct command *lex_cmd,
                          const struct command_info *info[])
{

    char *first_word = lex_cmd->argv[0];
    if(!first_word || strlen(first_word) == 0)
       return;

    out->type = define_command_type(first_word, info); 
    out->info = info[out->type];
    if(out->type != CMD_UNKNOWN && lex_cmd->argc < out->info->min_argc+1)
        return;

    out->is_correct = 0;
    switch(out->type)
    {
        case CMD_MOVE:
            parse_move_args(out, lex_cmd); 
            break;
        case CMD_SQUAD_INFO_UNIT:
            parse_num(&out->is_correct, lex_cmd->argv[1], 
                                    &out->args.unit_num);
            break;
        case CMD_SQUAD_INFO_ITEM:
        case CMD_SHOP_SELL:
            parse_num(&out->is_correct, lex_cmd->argv[1], 
                                    &out->args.item_num);
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
        case CMD_SHOP_INFO_ITEM:
        case CMD_SHOP_BUY:
            parse_num(&out->is_correct, lex_cmd->argv[1], 
                               &out->args.shop_item_num);
            break;
        default:
            out->is_correct = 1;
            break;
    }
}
