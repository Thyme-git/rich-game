# include <malloc.h>
# include "utils.h"

extern const char role_symbol[ROLE_NUM];

Privilige_t* func_init_privilige()
{
    Privilige_t* ptr = (Privilige_t*)malloc(sizeof(Privilige_t));
    ptr->next = NULL;
    return ptr;
}

void func_pop(Privilige_t* ptr, Role_enum role)
{
    Privilige_t* curr = ptr;
    while (curr->next)
    {
        if (curr->next->role == role)
        {
            break;
        }
        curr = curr->next;
    }
    if (curr->next == NULL)
    {
        return;
    }
    Privilige_t* tmp = curr->next;
    curr->next = curr->next->next;
    free(tmp);
}

void func_push(Privilige_t* ptr, Role_enum role)
{
    Privilige_t* curr = ptr;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    Privilige_t* new = func_init_privilige();
    new->role = role;
    curr->next = new;
}

Role_enum func_front(Privilige_t* ptr)
{
    if (ptr->next == NULL)
    {
        return -1;
    }
    return ptr->next->role;
}

Role_enum func_tail(Privilige_t* ptr)
{
    Privilige_t* curr = ptr;
    while (curr->next != NULL)
    {
        curr = curr->next;
    }
    return curr->role;
}

void func_free_pri(Privilige_t* ptr)
{
    Privilige_t* curr = ptr;
    while (curr)
    {
        Privilige_t* tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}

void func_get_pri_order(Privilige_t* ptr, char buf[])
{
    Privilige_t* curr = ptr->next;
    int cnt = 0;
    while(curr)
    {
        buf[cnt++] = role_symbol[curr->role];
        curr = curr->next;
    }
    buf[cnt] = '\0';
}