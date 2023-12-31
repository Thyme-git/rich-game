# include "utils.h"

extern const char role_symbol[ROLE_NUM];
FILE* dump_fp = NULL;

void func_dump(Game_t *game_ptr, int player_id, FILE *fin)
{
    if (fin == NULL)
    {
        printf("没有指定文件进行存储！\n");
        return;
    }

    Player_t** player_ptr = game_ptr->players_ptr;
    Land_t** land_ptr = game_ptr->land_ptr;
    fprintf(fin,"%s","user ");
    for (int player_id_ = 0; player_id_ < game_ptr->player_num; ++player_id_)
    {
        fprintf(fin,"%c",role_symbol[player_ptr[player_id_]->role]);  
        fflush(fin);
    }

    fprintf(fin,"\npreuser %c\n",role_symbol[player_ptr[player_id]->role]);
    for (Role_enum role = ROLE_Q; role < ROLE_NUM; ++role)
    {
        int exist = 0;
        for (int i = 0; i < game_ptr->player_num; ++i)
        {
            if (player_ptr[i]->role == role){
                exist = 1;
                fprintf(fin,"%c\n",role_symbol[player_ptr[i]->role]);
                fprintf(fin,"alive %d\n",player_ptr[i]->lose?0:1);
                fprintf(fin,"money %d\n",player_ptr[i]->money);
                fprintf(fin,"point %d\n",player_ptr[i]->point);
                fprintf(fin,"item1 %d\n",player_ptr[i]->barrier_cnt);
                fprintf(fin,"item2 %d\n",player_ptr[i]->robot_cnt);
                fprintf(fin,"item3 %d\n",0); // 可能保持零
                fprintf(fin,"buff %d\n",player_ptr[i]->free_of_toll_cnt);
                fprintf(fin,"stop %d\n", player_ptr[i]->recovery_time_cnt);
                fprintf(fin,"userloc %d\n",player_ptr[i]->pos);
                fflush(fin);
            }
        }
        if (!exist)
        {
            fprintf(fin,"%c\n",role_symbol[role]);
            fprintf(fin,"alive %d\n",0);
            fprintf(fin,"money %d\n",0);
            fprintf(fin,"point %d\n",0);
            fprintf(fin,"item1 %d\n",0);
            fprintf(fin,"item2 %d\n",0);
            fprintf(fin,"item3 %d\n",0);
            fprintf(fin,"buff %d\n",0);
            fprintf(fin,"stop %d\n",0);
            fprintf(fin,"userloc %d\n",0);
            fflush(fin); 
        }
    }
    fprintf(fin, "MAP");
    // TODO
    /*
    记录用户顺序;
    */
    func_update_map_info(game_ptr, player_id);
    for (int i = 0; i < LAND_NUM; ++i){
        if (func_check_someone_here(game_ptr, i))
        {
            char buf[ROLE_NUM];
            func_get_pri_order(land_ptr[i]->privilige_role, buf);
            fprintf(fin, "\nmapuser %d %s", i, buf);
            // putchar('\n');
        }
    }

    for(int land_pos=0;land_pos<LAND_NUM;land_pos++)
    {
        if(land_ptr[land_pos]->owner_id!=-1)
        {
            Role_enum role = game_ptr->players_ptr[land_ptr[land_pos]->owner_id]->role;
            fprintf(fin,"\nmap %d %c %d",land_pos,role_symbol[role],land_ptr[land_pos]->type);
            fflush(fin);
        }
    }
    for(int land_pos=0;land_pos<LAND_NUM;land_pos++)
    {
        if(land_ptr[land_pos]->item != VOID_ITEM && land_ptr[land_pos]->item != BUFF)
        {
            fprintf(fin,"\nitem %d %d",land_pos,land_ptr[land_pos]->item);
            fflush(fin);
        }
    }

    for(int land_pos=0;land_pos<LAND_NUM;land_pos++)
    {
        if(land_ptr[land_pos]->item == BUFF)
        {
            fprintf(fin,"\nmapgod %d %d",land_pos,5-game_ptr->buff_keep_time);
            fflush(fin);
        }
    }
}

int func_check_someone_here(Game_t* game_ptr, int pos)
{
    for (int i = 0; i < game_ptr->player_num; ++i)
    {
        if (game_ptr->players_ptr[i]->pos == pos && game_ptr->players_ptr[i]->lose == 0)
        {
            return 1;
        }
    }
    return 0;
}

int func_check_is_player(char c)
{
    return c == 'Q' || c == 'A' || c == 'S' || c == 'J';
}