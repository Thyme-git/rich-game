# include "utils.h"

extern const char role_symbol[ROLE_NUM];
FILE* dump_fp = NULL;

void func_dump(Game_t *game_ptr,int player_id, FILE *fin)
{
    Player_t** player_ptr = game_ptr->players_ptr;
    Land_t** land_ptr = game_ptr->land_ptr;
    fprintf(fin,"%s","user ");
    for (int player_id_ = 0; player_id_ < game_ptr->player_num; ++player_id_)
    {
        fprintf(fin,"%c",role_symbol[player_ptr[player_id_]->role]);  
        fflush(fin);
    }
    fprintf(fin,"\npreuser %c\n",role_symbol[player_ptr[player_id]->role]);
    for (int player_id_ = 0; player_id_ < game_ptr->player_num; ++player_id_)
    {
        fprintf(fin,"%c\n",role_symbol[player_ptr[player_id_]->role]);
        fprintf(fin,"alive %d\n",player_ptr[player_id_]->lose?0:1);
        fprintf(fin,"money %d\n",player_ptr[player_id_]->money);
        fprintf(fin,"item1 %d\n",player_ptr[player_id_]->barrier_cnt);
        fprintf(fin,"item2 %d\n",player_ptr[player_id_]->robot_cnt);
        fprintf(fin,"item3 %d\n",player_ptr[player_id_]->bomb_cnt);
        fprintf(fin,"buff %d\n",player_ptr[player_id_]->free_of_toll_cnt);
        fprintf(fin,"stop %d\n",player_ptr[player_id_]->recovery_time_cnt);
        fprintf(fin,"user_loc %d\n",player_ptr[player_id_]->pos);
        fflush(fin);
    }
    fprintf(fin,"%s\n","MAP");
    // TODO
    /*
    记录用户顺序;
    */

    for(int land_pos=0;land_pos<LAND_NUM;land_pos++)
    {
        if(land_ptr[land_pos]->owner_id!=-1)
        {
            fprintf(fin,"map %d %c %d",land_pos,role_symbol[land_ptr[land_pos]->owner_id],land_ptr[land_pos]->type);
            fflush(fin);
        }
    }
    for(int land_pos=0;land_pos<LAND_NUM;land_pos++)
    {
        if(land_ptr[land_pos]->item!=VOID_ITEM)
        {
            fprintf(fin,"item %d %d",land_pos,land_ptr[land_pos]->item);
            fflush(fin);
        }
    }

}