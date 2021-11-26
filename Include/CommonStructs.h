#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

using env_vars = unordered_map<string, string>;
using dir_list = vector<string>;

typedef struct wamr_config{
    int log_verbose_lvl;
    int max_thread_num;
    int stack_size;
    int heap_size;
    env_vars environment_variables;
    dir_list directory_list;

} wamr_config_t;



#endif //COMMON_STRUCTS_H