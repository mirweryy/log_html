#pragma once 
#include <fstream>
#include <iostream>
#include <cstddef>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <stdlib.h>
#include <math.h>

// struct config_parse
// {
    
// };

class parserlog
{
public:
    parserlog() = delete;
    parserlog(char* path);
    bool parse_header(char split_char);
    void parse_log();
    void create_html_table();
    void create_html_table2();
    void add_html_row(char* time, char* type, char* info);
    void close_html_row();
    void create_table_trace();
    void create_table_warning();
    void create_table_info();
    void create_table_debug();
    void create_table_all();
    void create_table_unspec();
    void create_table_critical();
    void create_table_error();
    void close_html_row2();
    void create_summary_data();
    void create_timeline_data();
    //parselog(char* path, )
private:
    int minute;
    size_t _count_char;
    char _buffer[1024];
    size_t _size_buffer = 1024;
    std::ifstream* _file;
    size_t _count_type[8];
    int mil_start_end = 0;
    int _min_send_interval[8];
    int _max_send_interval[8];
    int _first_send_data_time[8];
    char min_time[13] = "23:59:59.999";
    char max_time[13] = "00:00:00.000";
};
