#include <../include/parselib.h>

enum Type_log
{
  TRACE, 
  WARNING, 
  DEBUG,
  INFO,
  CRITICAL,
  ERROR,
  UNSPEC,
  ALL
};

void create_dir_out()
{
    if (mkdir("outdir", 0777) == -1)
    {
        std::cout << "Error, create outdir \n";
    }
}

int time_in_mill(const char* time1)
{
    char copy_time1[13];
    memcpy(copy_time1, time1, 13);
    char* firt_point = strchr(copy_time1, ':');
    *firt_point = '\0';
    ++firt_point;
    char* second_point = strchr(firt_point, ':');
    *second_point = '\0';
    ++second_point;
    char* third_point = strchr(second_point, '.');
    *third_point = '\0';
    ++third_point;
    int hour = atoi(copy_time1);
    int min = atoi(firt_point);
    int sec = atoi(second_point);
    int mil = atoi(third_point);
    //std::cout << hour << " " << min << " " << sec << " " << mil << std::endl;
    int all_mil = hour * 60 * 60 * 1000 + min * 60 * 1000 + sec * 1000 + mil;
    return all_mil;
}

int time_hour_min(const char* time1)
{
    char copy_time1[13];
    memcpy(copy_time1, time1, 13);
    char* firt_point = strchr(copy_time1, ':');
    *firt_point = '\0';
    ++firt_point;
    char* second_point = strchr(firt_point, ':');
    *second_point = '\0';
    int hour = atoi(copy_time1);
    int min = atoi(firt_point);
    //std::cout << hour << " " << min << " " << sec << " " << mil << std::endl;
    int all_mil = (hour * 60 * 60 * 1000) + (min * 60 * 1000);
    return all_mil;
}

int time_hour_min_sec(const char* time1)
{
    char copy_time1[13];
    memcpy(copy_time1, time1, 13);
    char* firt_point = strchr(copy_time1, ':');
    *firt_point = '\0';
    ++firt_point;
    char* second_point = strchr(firt_point, ':');
    *second_point = '\0';
    ++second_point;
    char* third_point = strchr(second_point, '.');
    *third_point = '\0';
    int hour = atoi(copy_time1);
    int min = atoi(firt_point);
    int sec = atoi(second_point);
    //std::cout << hour << " " << min << " " << sec << " " << mil << std::endl;
    int all_mil = hour * 60 * 60 * 1000 + min * 60 * 1000 + (int(sec / 10) * 10000);
    return all_mil;
}

int len_str(char* str)
{
    int index = 0;
    while (str[index] != '\0')
    {
        ++index;
    }
    return index;
}

int comparison_data(char* time1, char* time2)
{
    if (len_str(time1) != len_str(time2))
    {
      return 0;
    }
    while (*time1 != '\0')
    {
      if (*time1 != ':' || *time1 != '.')
      {
        int num1 = *time1 - '0';
        int num2 = *time2 - '0';
        if (num1 > num2)
        {
          return 1;
        }
        else if (num1 < num2)
        {
          return -1;
        }
      }
      ++time1;
      ++time2;
    }
    return 2;
  // если время 1 больше времени 2, то получим 1. Если время 1 меньше, чем время 2, то получим -1;
};


parserlog::parserlog(char* path)
{
    _file = new std::ifstream(path,  std::ios::in | std::ios::ate);
    if (!_file->is_open())
    {
      throw "Error, create object";
    }
    size_t _count_char = _file->tellg();
    _file->seekg(0);
    create_dir_out();
    //_file->getline(_buffer, _size_buffer, '\n');
    //std::cout << _buffer;
    if (parse_header('=') == 1)
    {
      parse_log();
    }
    else
    {
      std::cout << "Error, parse log" << std::endl;
      throw "Error, parse log";
    }
}

bool parserlog::parse_header(char split_char)
{
    std::ofstream outfile("outdir/header.txt", std::ios::out);
    if (!outfile.is_open())
    {
      throw "Error, open header.txt";
    }
    bool first_find = 0;
    bool second_find = 0;
    do{
        _file->getline(_buffer, _size_buffer);
        if (_buffer[0] != split_char && first_find != 1)
        {
            return 0;
        }
        int index = 0;
        bool all_split_char = 0;
        while (_buffer[index] != '\0')
        {
            if (_buffer[index] == split_char)
            {
                all_split_char = 1;
            }
            else
            {
                all_split_char = 0;
            }
            ++index;
        }
        if (first_find == 0 && all_split_char == 1)
        {
            first_find = 1;
        }
        else if (first_find == 1 && all_split_char == 1)
        {
            second_find = 1;
        }
        outfile.write(_buffer, len_str(_buffer));
        outfile.write("\n", 1);
    }while(second_find != 1);
    outfile.close();
    return 1;
}

void interval_set(int first_time, int second_time, int* current_time_interval_min, int* current_time_interval_max)
{
  if (*current_time_interval_min == -1)
  {
    *current_time_interval_min = first_time - second_time;
  }
  else if (first_time - second_time < *current_time_interval_min)
  {
    *current_time_interval_min = first_time - second_time; 
  }
  if (*current_time_interval_max == -1)
  {
    *current_time_interval_max = first_time - second_time;
  }
  else if (first_time - second_time > *current_time_interval_max)
  {
    *current_time_interval_max = first_time - second_time; 
  }
}




void parserlog::parse_log()
{
    std::ofstream outfile("outdir/log.txt", std::ios::out);
    if (!outfile.is_open())
    {
      throw "Error, open log.txt";
    }
    std::ofstream tracefile("outdir/trace.txt", std::ios::out);
    if (!tracefile.is_open())
    {
      throw "Error, open trace.txt";
    }
    std::ofstream warningfile("outdir/warning.txt", std::ios::out);
    if (!warningfile.is_open())
    {
      throw "Error, open warning.txt";
    }
    std::ofstream debugfile("outdir/debug.txt", std::ios::out);
    if (!debugfile.is_open())
    {
      throw "Error, open debug.txt";
    }
    std::ofstream infofile("outdir/info.txt", std::ios::out);
    if (!infofile.is_open())
    {
      throw "Error, open info.txt";
    }
    std::ofstream criticalfile("outdir/critical.txt", std::ios::out);
    if (!criticalfile.is_open())
    {
      throw "Error, open critical.txt";
    }
    std::ofstream errorfile("outdir/error.txt", std::ios::out);
    if (!errorfile.is_open())
    {
      throw "Error, open error.txt";
    }
    std::ofstream unspecfile("outdir/unspec.txt", std::ios::out);
    if (!unspecfile.is_open())
    {
      throw "Error, open unspec.txt";
    }
    std::ofstream data_line("outdir/data_line.txt", std::ios::out);
    if (!data_line.is_open())
    {
      throw "Error, open data_line.txt";
    }
    std::ofstream data_graf_min("outdir/data_grafmin.txt", std::ios::out);
    if (!data_graf_min.is_open())
    {
      throw "Error, open debug.txt";
    }
    //std::ofstream errorfile("outdir/eror.txt", std::ios::out);
    //std::ofstream timefile("outdir/time.txt", std::ios::out);
    for (int i = 0; i < 8; ++i)
    {
      _count_type[i] = 0;
      _first_send_data_time[i] = -1;
      _max_send_interval[i] = -1;
      _max_send_interval[i] = -1;
    }
    create_html_table2();
    //create_html_table();
    char type[128] = "None type";
    char info_type[1024] = "None info";
    int start_time_mill = -1; 
    int minut_count_type[8];
    minute = 0;
    while(_file->getline(_buffer, _size_buffer))
    {
        ++_count_type[ALL];
        outfile.write(_buffer, len_str(_buffer));
        char temp_buffer[_size_buffer];
        strcpy(temp_buffer, _buffer);
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        // if (min_time == nullptr)
        // {
        //   memcpy(min_time, temp_buffer, len_str(temp_buffer));
        // }
        //std::cout << min_time << std::endl;
        if (comparison_data(min_time, temp_buffer) == 1)
        {
          memcpy(min_time, temp_buffer, len_str(temp_buffer));
        }
        if (comparison_data(max_time, temp_buffer) == -1)
        {
          memcpy(max_time, temp_buffer, len_str(temp_buffer));
        }
        if (strcmp(type, first_pos) != 0)
        {
          if (strcmp(info_type, "None info") != 0)
          {
            data_line.write(info_type, len_str(info_type));
            data_line.write("\n", 1);
          }
          memcpy(type, first_pos, len_str(first_pos));
          type[len_str(first_pos)] = '\0';
          data_line.write(_buffer, len_str(_buffer));
          data_line.write("\n", 1);
        }
        if (start_time_mill == -1)
        {
          //std::cout << start_time_mill << " " << time_hour_min(_buffer) << " " << _buffer << std::endl ;
          start_time_mill = time_hour_min(_buffer);
          std::string s0 = std::to_string(minute);
          data_graf_min.write(s0.c_str(), s0.length());
          data_graf_min.write((char*) ": ", 2);
          data_graf_min.write("[ ", 2);
          //data_graf_min.write("--------------------------------------------\n", len_str("--------------------------------------------\n"));
          for (int i = 0; i < 8; ++i)
          {
            std::string s = std::to_string(0);
            minut_count_type[i] = 0;
            data_graf_min.write(s.c_str(), s.length());
            if (i != 7)
            {
              data_graf_min.write(", ", 2);
            }
          }
          data_graf_min.write("],", 2);
          ++minute;
          data_graf_min.write("\n", 1);
        }
        memcpy(info_type, _buffer, len_str(_buffer));
        info_type[len_str(_buffer)] = '\0';\
        
        if ((start_time_mill + 60000) < time_in_mill(_buffer))
        {
          //std::cout << start_time_mill << " " << time_in_mill(_buffer) << " " << _buffer << std::endl ;
          if ((start_time_mill + 60000) < time_in_mill(_buffer) && (start_time_mill + 120000) > time_in_mill(_buffer))
          {
            start_time_mill = start_time_mill + 60000;
            std::string s0 = std::to_string(minute);
            data_graf_min.write(s0.c_str(), s0.length());
            data_graf_min.write((char*) ": ", 2);
            data_graf_min.write("[ ", 2);
            //data_graf_min.write("--------------------------------------------\n", len_str("--------------------------------------------\n"));
            for (int i = 0; i < 8; ++i)
            {
              std::string s = std::to_string(minut_count_type[i]);
              data_graf_min.write(s.c_str(), s.length());
              if (i != 7)
              {
                data_graf_min.write(", ", 2);
              }
              minut_count_type[i] = 0;
            }
            data_graf_min.write("],", 2);
            ++minute;
            data_graf_min.write("\n", 1);
          }
          else
          {
            while (!((start_time_mill) < time_in_mill(_buffer) && (start_time_mill + 60000) > time_in_mill(_buffer)))
            {
              start_time_mill += 60000;
              std::string s0 = std::to_string(minute);
              data_graf_min.write(s0.c_str(), s0.length());
              data_graf_min.write((char*) ": ", 2);
              data_graf_min.write("[ ", 2);
              //data_graf_min.write("--------------------------------------------\n", len_str("--------------------------------------------\n"));
              for (int i = 0; i < 8; ++i)
              {
                std::string s = std::to_string(minut_count_type[i]);
                data_graf_min.write(s.c_str(), s.length());
                if (i != 7)
                {
                  data_graf_min.write(", ", 2);
                }
                minut_count_type[i] = 0;
              }
              data_graf_min.write("],", 2);
              ++minute;
              data_graf_min.write("\n", 1);
            }
          }
        }
        //std::cout << type << std::endl;
        // std::cout << min_time << std::endl;
        //timefile.write(_buffer, len_str(_buffer));
        //timefile.write("\n", 1);
        //errorfile.write(first_pos, len_str(first_pos));
        //errorfile.write("\n", 1);
        outfile.write("\n", 1);
        ++minut_count_type[ALL];
        if (strcmp(first_pos, "TRACE") == 0)
        {
            tracefile.write(_buffer, len_str(_buffer));
            tracefile.write("\n", 1);
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            ++_count_type[TRACE];
            ++minut_count_type[TRACE];
            if (_first_send_data_time[TRACE] == -1)
            {
              _first_send_data_time[TRACE] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[TRACE], &_min_send_interval[TRACE], &_max_send_interval[TRACE]);
              _first_send_data_time[TRACE] = time_in_mill(_buffer);
            }
        }
        else if (strcmp(first_pos, "DEBUG") == 0)
        {
            //add_html_row(_buffer, first_pos, ++second_pos);
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            debugfile.write(_buffer, len_str(_buffer));
            debugfile.write("\n", 1);
            ++_count_type[DEBUG];
            ++minut_count_type[DEBUG];
            if (_first_send_data_time[DEBUG] == -1)
            {
              _first_send_data_time[DEBUG] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[DEBUG], &_min_send_interval[DEBUG], &_max_send_interval[DEBUG]);
              _first_send_data_time[DEBUG] = time_in_mill(_buffer);
            }
        }
        else if (strcmp(first_pos, "INFO") == 0)
        {
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            infofile.write(_buffer, len_str(_buffer));
            infofile.write("\n", 1);
            ++_count_type[INFO];
            ++minut_count_type[INFO];
            if (_first_send_data_time[INFO] == -1)
            {
              _first_send_data_time[INFO] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[INFO], &_min_send_interval[INFO], &_max_send_interval[INFO]);
              _first_send_data_time[INFO] = time_in_mill(_buffer);
            }
        }
        else if (strcmp(first_pos, "WARNING") == 0)
        {
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            warningfile.write(_buffer, len_str(_buffer));
            warningfile.write("\n", 1);
            ++_count_type[WARNING];
            ++minut_count_type[WARNING];
            if (_first_send_data_time[WARNING] == -1)
            {
              _first_send_data_time[WARNING] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[WARNING], &_min_send_interval[WARNING], &_max_send_interval[WARNING]);
              _first_send_data_time[WARNING] = time_in_mill(_buffer);
            }
        } 
        else if (strcmp(first_pos, "CRITICAL") == 0)
        {
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            criticalfile.write(_buffer, len_str(_buffer));
            criticalfile.write("\n", 1);
            ++_count_type[CRITICAL];
            ++minut_count_type[CRITICAL];
            if (_first_send_data_time[CRITICAL] == -1)
            {
              _first_send_data_time[CRITICAL] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[CRITICAL], &_min_send_interval[CRITICAL], &_max_send_interval[CRITICAL]);
              _first_send_data_time[CRITICAL] = time_in_mill(_buffer);
            }
        }
        else if (strcmp(first_pos, "ERROR") == 0)
        {
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            errorfile.write(_buffer, len_str(_buffer));
            errorfile.write("\n", 1);
            ++_count_type[ERROR];
            ++minut_count_type[ERROR];
            if (_first_send_data_time[ERROR] == -1)
            {
              _first_send_data_time[ERROR] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[ERROR], &_min_send_interval[ERROR], &_max_send_interval[ERROR]);
              _first_send_data_time[ERROR] = time_in_mill(_buffer);
            }
        }
        else
        {
            //add_html_row(temp_buffer, first_pos, ++second_pos);
            unspecfile.write(_buffer, len_str(_buffer));
            unspecfile.write("\n", 1);
            ++_count_type[UNSPEC];
            ++minut_count_type[UNSPEC];
            if (_first_send_data_time[UNSPEC] == -1)
            {
              _first_send_data_time[UNSPEC] = time_in_mill(_buffer);
            }
            else
            {
              interval_set(time_in_mill(_buffer), _first_send_data_time[UNSPEC], &_min_send_interval[UNSPEC], &_max_send_interval[UNSPEC]);
              _first_send_data_time[UNSPEC] = time_in_mill(_buffer);
            }
        }
    }
    if (strcmp(info_type, "None info") != 0)
    {
      data_line.write(info_type, len_str(info_type));
      data_line.write("\n", 1);
    }
    std::string s0 = std::to_string(minute);
    data_graf_min.write(s0.c_str(), s0.length());
    data_graf_min.write((char*) ": ", 2);
    data_graf_min.write("[ ", 2);
    //data_graf_min.write("--------------------------------------------\n", len_str("--------------------------------------------\n"));
    for (int i = 0; i < 8; ++i)
    {
      std::string s = std::to_string(minut_count_type[i]);
      data_graf_min.write(s.c_str(), s.length());
      if (i != 7)
      {
        data_graf_min.write(", ", 2);
      }
    }
    data_graf_min.write("],", 2);
    ++minute;
    data_graf_min.write("\n", 1);
    for (int i = 0; i < 8; ++i)
    {
      if (_max_send_interval[i] == -1)
      {
        _max_send_interval[i] = 0;
      }
      if (_min_send_interval[i] == -1)
      {
        _min_send_interval[i] = 0;
      }
    }  
    outfile.close();
    tracefile.close();
    warningfile.close();
    debugfile.close();
    infofile.close();
    criticalfile.close();
    errorfile.close();
    unspecfile.close();
    data_line.close();
    data_graf_min.close();
    create_table_error();
    create_table_info();
    create_table_warning();
    create_table_unspec();
    create_table_critical();
    create_table_debug();
    create_table_all();
    create_table_trace();
    create_summary_data();
    create_timeline_data();
    close_html_row2();
    //close_html_row();
    //std::cout << _count_type[0] << std::endl;
    mil_start_end = time_in_mill(max_time) - time_in_mill(min_time);
    //std::cout << _count_string << std::endl;
    std::cout << min_time << std::endl;
    std::cout << max_time << std::endl;
    std::cout << time_in_mill(max_time) - time_in_mill(min_time) << std::endl;
    std::cout << time_in_mill(max_time) << " " << start_time_mill << std::endl;
    std::cout << _min_send_interval[TRACE] << " " << _max_send_interval[TRACE] << std::endl;
    //std::cout << min_time << std::endl;
    //std::cout << min_send_interval << " " << max_send_interval;
    //std::cout << _count_trace + _count_critical + _count_debug + _count_error + _count_warning + _count_info << std::endl;
}


void parserlog::create_html_table()
{
    std::ofstream outfile("outdir/html.html", std::ios::out);
    if (!outfile.is_open())
    {
      throw "Error, open html.html";
    }
    char* base = (char*)"<table class=\"table\"><tr><th>Time</th><th>Type</th><th>Info</th></tr>";
    outfile.write(base, len_str(base));
    outfile.write("\n", 1);
    outfile.close();
}

void parserlog::add_html_row(char* time, char* type, char* info)
{
    std::ofstream outfile("outdir/html.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html.html";
    }
    outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
    outfile.write((char*)"<td>", len_str((char*)"<td>")); 
    outfile.write(time, len_str(time));
    outfile.write((char*)"</td>", len_str((char*)"</td>"));
    outfile.write((char*)"<td>", len_str((char*)"<td>")); 
    outfile.write(type, len_str(type));
    outfile.write((char*)"</td>", len_str((char*)"</td>"));
    outfile.write((char*)"<td>", len_str((char*)"<td>")); 
    outfile.write(info, len_str(info));
    outfile.write((char*)"</td>", len_str((char*)"</td>"));
    outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
    outfile.write("\n", 1);
}        

void parserlog::close_html_row()
{
  std::ofstream outfile("outdir/html.html", std::ios::app);
  if (!outfile.is_open())
  {
    throw "Error, open html.html";
  }
  char* style = (char*)"</table> <style type=\"text/css\">\
    .table { \
	width: 50%;\
	border: 1px;\
	margin-bottom: 20px;\
	border-collapse: separate;\
    }\
    .table thead th {\
	font-weight: bold;\
	text-align: left;\
	border: none;\
	padding: 10px 15px;\
	background: #7c6c6c;\
	font-size: 14px;\
	border-top: 1px solid #000000;\
    }\
    .table tr th:first-child, .table tr td:first-child {\
	border-left: 1px solid #000000;\
    }\
    .table tr th:last-child, .table tr td:last-child {\
	border-right: 1px solid #000000;\
    }\
    .table thead tr th:first-child {\
	border-radius: 20px 0 0 0;\
    }\
    .table thead tr th:last-child {\
	border-radius: 0 20px 0 0;\
    }\
    .table tbody td {\
	text-align: left;\
	border: 1px;\
	padding: 10px 15px;\
	font-size: 14px;\
	vertical-align: top;\
    }\
    .table tbody tr:nth-child(even) {\
	background: #F8F8F8;\
    }\
    .table tbody tr:last-child td{\
	border-bottom: 1px solid #ddd;\
    }\
    .table tbody tr:last-child td:first-child {\
	border-radius: 0 0 0 20px;\
    }\
    .table tbody tr:last-child td:last-child {\
	border-radius: 0 0 20px 0;\
    }\
    </style>";
    //char* javasc = (char*)"<button type=\"button\"onclick=\"document.getElementById('demo').innerHTML = Date()\">Click me to display Date and Time.</button>\"";
    // <button onclick="changeColor()">Изменить цвет</button>

    // <script>
    //   function changeColor() {
    //     var element = document.getElementById('t1');
    //     element.style.backgroundColor = 'lightblue';
    //   }
    outfile.write(style, len_str(style));
    outfile.write("\n", 1);
    //outfile.write(javasc, len_str(javasc));
    //outfile.write("\n", 1);
    outfile.close();
}

void parserlog::create_html_table2()
{
  std::ofstream outfile("outdir/html2.html", std::ios::out);
  if (!outfile.is_open())
  {
    throw "Error, open html2.html";
  }
  char* style = (char*)"<!DOCTYPE html>\n\
<html lang=\"ru\">\n\
<head>\n\
  <meta charset=\"UTF-8\">\n\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
  <title>Log</title>\n\
  <link rel=\"stylesheet\" href=\"style.css\">\n\
  <link rel=\"stylesheet\" href=\"colors.css\">\n\
</head>\n\
<body>\n\
<div id=\"canvasPointTooltip\" class=\"canvas-tooltip hidden\"></div>\n\
<div class=\"card\">\n\
  <h1>Log</h1>\n\
  <div class=\"subhead\"></div>\n\
  <div class=\"button-panel\">\n\
    <button class=\"table-btn\" data-table=\"1\">CRITICAL</button>\n\
    <button class=\"table-btn\" data-table=\"2\">DEBUG</button>\n\
    <button class=\"table-btn\" data-table=\"3\">TRACE</button>\n\
    <button class=\"table-btn\" data-table=\"4\">INFO</button>\n\
    <button class=\"table-btn\" data-table=\"5\">WARNING</button>\n\
    <button class=\"table-btn\" data-table=\"6\">ERROR</button>\n\
    <button class=\"table-btn\" data-table=\"7\">UNSPEC</button>\n\
    <button class=\"table-btn\" data-table=\"8\">ALL</button>\n\
    <button class=\"table-btn\" data-summary=\"true\">Общая сводка</button>\n\
    <button class=\"table-btn\" data-timeline=\"true\">Временная шкала</button>\n\
  </div>\n\
\n\
  <div class=\"table-wrapper\">\n\
    <div id=\"dynamicTableContainer\" class=\"table-container\">\n\
      <div class=\"placeholder-message\"></div>\n\
    </div>\n\
  </div>\n\
  <div class=\"footer-note\"></div>\n\
</div>\n\
\n\
<!-- Глобальный тултип -->\n\
<div id=\"globalTimelineTooltip\"></div>\n\
\n\
<script>\n\
  (function() {\n\
    \"use strict\";";
    outfile.write(style, len_str(style));
}

void parserlog::create_summary_data()
{
  char* temp_arr[8] = {"'TRACE',", "'WARNING',", "'DEBUG',", "'INFO',", "'CRITICAL',", "'ERROR',", "'UNSPEC',", "'ALL',"};
  std::ofstream outfile("outdir/html2.html", std::ios::app);
  if (!outfile.is_open())
  {
    throw "Error, open html2.html";
  }
  char* style = (char*)"const staticSummaryData = [\n\
  {\n\
    title: 'Количество строк',\n fields: [\n";
  outfile.write(style, len_str(style));
  for (int i = 0; i < 8; ++i)
  {
    std::string s0 = std::to_string(_count_type[i]);
    std::cout << _count_type[i] << std::endl;
    outfile.write((char*)"{ label: ",  len_str((char*)"{ label: "));
    outfile.write(temp_arr[i], len_str(temp_arr[i]));
    outfile.write((char*)("value: "), len_str((char*)("value: ")));
    outfile.write(s0.c_str(), s0.length());
    outfile.write((char*)" },\n", len_str((char*)" },\n"));
  }
  outfile.write((char*)"]\n },\n", len_str((char*)"]\n },\n"));
  char* style2 = (char*)"{\n\
     title: 'Количество строк в процентах',\n fields: [\n";
  outfile.write(style2, len_str(style2));
  for (int i = 0; i < 8; ++i)
  {
    std::string s0 = "'" + std::to_string(int(floor((float)(_count_type[i]) / _count_type[7] * 100))) + "%'";
    outfile.write((char*)"{ label: ",  len_str((char*)"{ label: "));
    outfile.write(temp_arr[i], len_str(temp_arr[i]));
    outfile.write((char*)("value: "), len_str((char*)("value: ")));
    outfile.write(s0.c_str(), s0.length());
    outfile.write((char*)" },\n", len_str((char*)" },\n"));
  }
  outfile.write((char*)"]\n },\n", len_str((char*)"]\n },\n"));
  char* style3 = (char*)"{\n\
     title: 'Максимальный интервал между сообщениями',\n fields: [\n";
  outfile.write(style3, len_str(style3));
  for (int i = 0; i < 7; ++i)
  {
    std::string s0 = "'";
    if (_max_send_interval[i] / 3600000 == 0)
    {
      s0 += "00:";
    } 
    else if (_max_send_interval[i] / 3600000 < 10)
    {
      s0 += "0" + std::to_string(_max_send_interval[i] / 3600000) + ":";
    }
    else
    {
      s0 += std::to_string(_max_send_interval[i] / 3600000) + ":";
    }
    if ((_max_send_interval[i] % 3600000) / 60000 == 0)
    {
      s0 += "00:";
    } 
    else if ((_max_send_interval[i] % 3600000) / 60000 < 10)
    {
      s0 += "0" + std::to_string((_max_send_interval[i] % 3600000) / 60000) + ":";
    }
    else
    {
      s0 += std::to_string((_max_send_interval[i] % 3600000) / 60000) + ":";
    }
    if ((_max_send_interval[i] % 60000 - _max_send_interval[i] % 1000) / 1000 == 0)
    {
      s0 += "00:";
    } 
    else if ((_max_send_interval[i] % 60000 - _max_send_interval[i] % 1000) / 1000 < 10)
    {
      s0 += "0" + std::to_string((_max_send_interval[i] % 60000 - _max_send_interval[i] % 1000) / 1000) + ".";
    }
    else
    {
      s0 += std::to_string((_max_send_interval[i] % 60000 - _max_send_interval[i] % 1000) / 1000) + ".";
    }
    if (_max_send_interval[i] % 1000 == 0)
    {
      s0 += "000'";
    } 
    else if (_max_send_interval[i] % 1000 < 10)
    {
      s0 += "00" + std::to_string(_max_send_interval[i] % 1000) + "'";
    }
    else if (_max_send_interval[i] % 1000 < 100)
    {
      s0 += "0" + std::to_string(_max_send_interval[i] % 1000) + "'";
    }
    else
    {
      s0 += std::to_string(_max_send_interval[i] % 1000) + "'";
    }
    outfile.write((char*)"{ label: ",  len_str((char*)"{ label: "));
    outfile.write(temp_arr[i], len_str(temp_arr[i]));
    outfile.write((char*)("value: "), len_str((char*)("value: ")));
    outfile.write(s0.c_str(), s0.length());
    outfile.write((char*)" },\n", len_str((char*)" },\n"));
  }
  outfile.write((char*)"]},\n];\n", len_str((char*)"]},\n];\n"));
//     ]\n\
//   },\n\
//   {\n\
//     title: 'Количество строк проценты',\n\
//     fields: [\n\
//       { label: 'ALL', value: '0%'},\n\
//       { label: 'CRITICAL', value: '0%' },\n\
//       { label: 'DEBUG', value: '0%' },\n\
//       { label: 'TRACE', value: '0%' },\n\
//       { label: 'INFO', value: '0%' },\n\
//       { label: 'WARNING', value: '0%' },\n\
//       { label: 'ERROR', value: '0%' }\n\
//     ]\n\
//   },\n\
//   {\n\
//     title: 'Временной анализ',\n\
//     fields: [\n\
//       { label: 'Продолжительность сессии', value: '17:00:11.999'},\n\
//     ]\n\
//   }\n\
// ];\n";
}

void parserlog::create_timeline_data()
{
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    std::ifstream infile("outdir/data_grafmin.txt", std::ios::in);
    if (!infile.is_open())
    {
      throw "Error, open data_grafmin.txt";
    }
    char temp_buffer[_size_buffer];
    char* style = (char*)"    let time_start = \"";
    outfile.write(style, len_str(style));
    outfile.write(min_time, 5);
    outfile.write("\"", 1);
    outfile.write("\n", 1);
    char* style2 = (char*)"const time_container = {\n";
    outfile.write(style2, len_str(style2));
    while(infile.getline(temp_buffer, _size_buffer))
    {
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write("\n", 1);
    }
    outfile.write("};", 2);
    outfile.write("\n", 1);
    outfile.write((char*) "let count_time = ", len_str((char*) "let count_time = "));
    std::string s0 = std::to_string(minute - 1);
    outfile.write(s0.c_str(), s0.length());
    outfile.write(";", 1);
    outfile.write("\n", 1);
    outfile.close();
    infile.close();
}


void parserlog::close_html_row2()
{
    std::ifstream source("config_data/colors.css", std::ios::binary);
    if (!source.is_open())
    {
      throw "Error, create colors.css";
    }
    std::ofstream dest("outdir/colors.css", std::ios::binary);
    if (!dest.is_open())
    {
      throw "Error, create colors.css";
    }
    dest << source.rdbuf();
    dest.close();
    source.close();
    std::ifstream source2("config_data/style.css", std::ios::binary);
    if (!source2.is_open())
    {
      throw "Error, create style.css";
    }
    std::ofstream dest2("outdir/style.css", std::ios::binary);
    if (!dest2.is_open())
    {
      throw "Error, create style.css";
    }
    dest2 << source2.rdbuf();
    dest2.close();
    source2.close();
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    std::ifstream infile("config_data/temp_end.txt", std::ios::in);
    if (!infile.is_open())
    {
      throw "Error, open temp_end.txt";
    }
    char temp_buffer[_size_buffer];
    while(infile.getline(temp_buffer, _size_buffer))
    {
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write("\n", 1);
    }
    outfile.close();
    infile.close();
    remove("outdir/trace.txt");
    remove("outdir/info.txt");
    remove("outdir/log.txt");
    remove("outdir/critical.txt");
    remove("outdir/header.txt");
    remove("outdir/error.txt");
    remove("outdir/data_line.txt");
    remove("outdir/data_grafmin.txt");
    remove("outdir/unspec.txt");
    remove("outdir/warning.txt");
    remove("outdir/debug.txt");
}

void parserlog::create_table_warning()
{
    std::ifstream file("outdir/warning.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open warning.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table5 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">WARNING</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"warning-row\">", len_str((char*)"<td class=\"warning-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}

void parserlog::create_table_trace()
{
    std::ifstream file("outdir/trace.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open trace.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table3 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">ALL</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"trace-row\">", len_str((char*)"<td class=\"trace-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}

void parserlog::create_table_info()
{
    std::ifstream file("outdir/info.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open info.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table4 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">INFO</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"info-row\">", len_str((char*)"<td class=\"info-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}

void parserlog::create_table_debug()
{
    std::ifstream file("outdir/debug.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open debug.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table2 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">DEBUG</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"debug-row\">", len_str((char*)"<td class=\"debug-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}

void parserlog::create_table_critical()
{
    std::ifstream file("outdir/critical.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open critical.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table1 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">CRITICAL</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"critical-row\">", len_str((char*)"<td class=\"critical-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}

void parserlog::create_table_error()
{
    std::ifstream file("outdir/error.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open error.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table6 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">ERROR</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"error-row\">", len_str((char*)"<td class=\"error-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}


void parserlog::create_table_all()
{
    std::ifstream file("outdir/log.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open log.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table8 = `` \n";
    outfile.write(start_table, len_str(start_table));
    outfile.close();
    file.close();
}

void parserlog::create_table_unspec()
{
    std::ifstream file("outdir/unspec.txt", std::ios::in);
    if (!file.is_open())
    {
      throw "Error, open unspec.txt";
    }
    std::ofstream outfile("outdir/html2.html", std::ios::app);
    if (!outfile.is_open())
    {
      throw "Error, open html2.html";
    }
    char temp_buffer[_size_buffer];
    // char* first_pos;
    // char* second_pos;
    char* start_table = (char*)"const table7 = `<div style=\"margin-bottom: 6px; font-weight: 500; color: #1e3b4f;\">UNSPEC</div>\n\
    <table>\n\
        <thead>\n\
          <tr><th>Time</th><th>Type</th><th>Info</th></tr>\n\
        </thead>\n\
        <tbody>\n";
    outfile.write(start_table, len_str(start_table));
    while(file.getline(temp_buffer, _size_buffer))
    {
        char* first_pos = strchr(temp_buffer, ' ');
        first_pos[0] = '\0';
        char* second_pos = strchr(++first_pos, ' ');
        second_pos[0] = '\0';
        ++second_pos;
        outfile.write((char*)"<tr>", len_str((char*)"<tr>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(temp_buffer, len_str(temp_buffer));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td class=\"all-row\">", len_str((char*)"<td class=\"all-row\">")); 
        outfile.write(first_pos, len_str(first_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"<td>", len_str((char*)"<td>")); 
        outfile.write(second_pos, len_str(second_pos));
        outfile.write((char*)"</td>", len_str((char*)"</td>"));
        outfile.write((char*)"</tr>", len_str((char*)"</tr>"));
        outfile.write("\n", 1);
    }
    char* end_table = (char*)"         </tbody>\n\
      </table>\n\
    `;\n";
    outfile.write(end_table, len_str(end_table));
    outfile.close();
    file.close();
}