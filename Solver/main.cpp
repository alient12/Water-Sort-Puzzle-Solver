#include "Container.h"
#include <vector>
#include <stack>
#include <array>
#include <stdlib.h>

using namespace std;

void show_containers(vector<Container>);
vector<vector<int>> check_possible_cases(vector<Container>);
void possible_cases_show(vector<vector<int>>);
vector<vector<size_t>> DFS(vector<Container>, vector<vector<size_t>>);
stack<array<size_t, 2>> DFS(vector<Container>, stack<array<size_t, 2>>);
void show_path(vector<vector<size_t>>);
void show_path(stack<array<size_t, 2>>);
void show_solution_steps(vector<Container>, stack<array<size_t, 2>>);
void show_path_for_user(stack<array<size_t, 2>>);

int main(int argc, char **argv)
{

    vector<Container> container_list{};
    size_t num_containers{(size_t)(argc-1)/4};
    size_t argv_counter{2};

    for (size_t i{}; i < num_containers; i++)
    {
        int arr[4];
        for (size_t j{}; j < 4; j++)
        {
            arr[j] = (int)strtol(argv[argv_counter], NULL, 10);
            argv_counter++;
        }
        Container cont {arr};
        container_list.push_back(cont);
    }

    // vector<Container> container_list{};
    // cout << "Enter number of containers :";
    // size_t num_containers{};
    // cin >> num_containers;
    // for (size_t i{}; i < num_containers; i++)
    // {
    //     int arr[4];
    //     for (size_t j{}; j < 4; j++)
    //     {
    //         cout << "Enter " << j << "th color of " << i << "th container from down to up :";
    //         cin >> arr[j];
    //     }
    //     Container cont {arr};
    //     container_list.push_back(cont);
    // }

    // vector<vector<size_t>> path_vec{{0, 0}};
    // path_vec = DFS(container_list, path_vec);
    // show_path(path_vec);

    stack<array<size_t, 2>> path;
    path = DFS(container_list, path);
    show_path_for_user(path);
    show_solution_steps(container_list, path);
    // show_containers(container_list);
}

void show_containers(vector<Container> container_list)
{
    // for (size_t i{}; i< container_list.size(); i++)
    // {
    //     cout << "#" << i << ":" << endl;
    //     container_list[i].show();
    // }
    for (int i{3}; i >= 0; i--)
    {
        for (size_t j{}; j < container_list.size(); j++)
        {
            if (container_list[j].get_colors(i) == -1)
                cout << " [ ]";
            else
                cout << " [" << container_list[j].get_colors(i) << "]";
        }
        cout << endl;
    }
    for (size_t i{}; i < container_list.size(); i++)
    {
        string temp{""};
        // if (container_list[i].get_colors(0) > 9 || container_list[i].get_colors(1) > 9 || container_list[i].get_colors(2) > 9 || container_list[i].get_colors(3) > 9)
        //     temp = " ";
        if (i + 1 > 9)
            cout << "  " << i + 1 << temp;
        else
            cout << "  " << i + 1 << " " << temp;
    }
    cout << endl;
}

vector<vector<int>> check_possible_cases(vector<Container> container_list)
{
    vector<vector<int>> possible_cases;
    for (size_t i{}; i<container_list.size(); i++)
    {
        possible_cases.push_back(vector<int>{-1});
        for (size_t j{}; j < container_list.size(); j++)
        {
            if(container_list[j].is_pourable_by(container_list[i]) && !container_list[j].is_fool_pour(container_list[i]))
            {
                possible_cases[i].push_back(j);
            }
        }
    }
    return possible_cases;
}

void possible_cases_show(vector<vector<int>> possible_cases)
{
    cout << "possible cases:" << endl;
    for (size_t i{}; i < possible_cases.size(); i++)
    {
        for (size_t j{}; j<possible_cases[i].size(); j++)
        {
            cout << possible_cases[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<size_t>> DFS(vector<Container> container_list, vector<vector<size_t>> path)
{
    //show path each loop (for debug purpose)

    // show_path(path);

    // check for success
    bool success_flag{true};
    for (size_t i{}; i < container_list.size(); i++)
    {
        if(!container_list[i].is_full_of_one_color())
        {
            success_flag = false;
            break;
        }
    }
    if (success_flag)
    {
        // cout << "success" << endl;
        path[0][0] = 1;
        path[0][1] = 1;
        return path;
    }

    // check for infinite loop
    if (path.size() > 2)
    {
        vector<size_t> last_path{path[path.size() - 1]};
        vector<size_t> penult_path{path[path.size() - 2]};
        if(last_path[0] == penult_path[1] && last_path[1] == penult_path[0])
        {
            path[0][0] = 0;
            // cout << "infinite loop" << endl;
            return path;
        }
    }

    vector<vector<int>> possible_cases{check_possible_cases(container_list)};

    //check for no move
    bool no_move_flag{true};
    for(size_t i{0}; i<possible_cases.size(); i++)
    {
        if(possible_cases[i].size() != 1)
        {
            no_move_flag = false;
            break;
        }
    }
    if (no_move_flag)
    {
        path[0][0] = 0;
        // cout << "no move" << endl;
        return path;
    }

    //make new branches
    for (size_t i{}; i < possible_cases.size(); i++)
    {
        if (possible_cases[i].size() == 1)
        {
            continue;
        }
        for (size_t j{1}; j < possible_cases[i].size(); j++)
        {
            vector<Container> new_container_list{container_list};
            new_container_list[possible_cases[i][j]].pour_by(new_container_list[i]);
            vector<vector<size_t>> new_path{path};
            vector<size_t> temp{(size_t)possible_cases[i][j], i};
            new_path.push_back(temp);
            vector<vector<size_t>>temp_path{DFS(new_container_list, new_path)};
            if (temp_path[0][0] == 1)
            {
                return temp_path;
            }
        }
    }
    return path;
}

stack<array<size_t, 2>> DFS(vector<Container> container_list, stack<array<size_t, 2>> path)
{
    // show path each loop (for debug purpose)

    // show_path(path);

    // check for success
    bool success_flag{true};
    for (size_t i{}; i < container_list.size(); i++)
    {
        if(!container_list[i].is_full_of_one_color())
        {
            success_flag = false;
            // cout << "No Success Yet" << endl;
            break;
        }
    }
    if (success_flag)
    {
        // cout << "success" << endl;
        array<size_t, 2> temp{1, 1};
        path.push(temp);
        // cout << "Success" << endl;
        return path;
    }

    // check for infinite loop
    if (path.size() > 1)
    {
        array<size_t, 2> last_path{path.top()};
        path.pop();
        array<size_t, 2> penult_path{path.top()};
        path.push(last_path);
        if (last_path[0] == penult_path[1] && last_path[1] == penult_path[0])
        {
            // cout << "infinite loop" << endl;
            array<size_t, 2> temp{0, 0};
            path.push(temp);
            // cout << "Infinite Loop" << endl;
            return path;
        }
    }

    vector<vector<int>> possible_cases{check_possible_cases(container_list)};

    //check for no move
    bool no_move_flag{true};
    for(size_t i{0}; i<possible_cases.size(); i++)
    {
        if(possible_cases[i].size() != 1)
        {
            no_move_flag = false;
            // cout << "No No Move Yet" << endl;
            break;
        }
    }
    if (no_move_flag)
    {
        // cout << "no move" << endl;
        array<size_t, 2> temp{0, 0};
        path.push(temp);
        // cout << "No Move" << endl;
        return path;
    }

    //make new branches
    // cout << "Start Make New Branch" << endl;
    for (size_t i{}; i < possible_cases.size(); i++)
    {
        if (possible_cases[i].size() == 1)
        {
            continue;
        }
        for (size_t j{1}; j < possible_cases[i].size(); j++)
        {
            vector<Container> new_container_list{container_list};
            new_container_list[possible_cases[i][j]].pour_by(new_container_list[i]);
            stack<array<size_t, 2>> new_path{path};
            array<size_t, 2> temp{(size_t)possible_cases[i][j], i};
            new_path.push(temp);
            stack<array<size_t, 2>>temp_path{DFS(new_container_list, new_path)};
            if (temp_path.top()[0] == 1 && temp_path.top()[1] == 1)
            {
                return temp_path;
            }
        }
    }
    // cout << "Finish Make New Branch" << endl;
    return path;
}

void show_path(vector<vector<size_t>> path)
{
    cout << "Path:" << endl;
    for (size_t i{}; i < path.size(); i++)
    {
        cout << "(" << path[i][0] << ", " << path[i][1] << ")";
    }
    cout << endl;
}

void show_path(stack<array<size_t, 2>> path)
{
    cout << "Path:" << endl;
    while(!path.empty())
    {
        cout << "(" << path.top()[0] << ", " << path.top()[1] << ")";
        path.pop();
    }
    cout << endl;
}

void show_solution_steps(vector<Container> container_list, stack<array<size_t, 2>> path)
{
    if (path.size() > 0)
    {
        path.pop();
        stack<array<size_t, 2>> path_temp;
        while(!path.empty())
        {
            path_temp.push(path.top());
            path.pop();
        }
        show_containers(container_list);
        size_t step{};
        while (!path_temp.empty())
        {
            container_list[path_temp.top()[0]].pour_by(container_list[path_temp.top()[1]]);
            path_temp.pop();
            step++;
            cout << "\nStep " << step << endl;
            show_containers(container_list);
        }
    }
}

void show_path_for_user(stack<array<size_t, 2>> path)
{
    if (path.size() > 0)
    {
        path.pop();
        array<size_t, 2> temp{0, 0};
        vector<array<size_t, 2>> vec;
        for (size_t i{}; i < path.size(); i++)
            vec.push_back(temp);
        size_t i{0};
        while (!path.empty())
        {
            array<size_t, 2> temp{path.top()};
            path.pop();
            temp[0]++;
            temp[1]++;
            vec[vec.size() - i - 1] = temp;
            i++;
        }
        cout << "Path:" << endl;
        for (size_t i{}; i < vec.size(); i++)
        {
            cout << "(" << vec[i][1] << ", " << vec[i][0] << ")";
        }
        cout << endl;
    }
}
