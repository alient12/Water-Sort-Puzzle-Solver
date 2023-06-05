#include "Container.h"
#include <iostream>

Container::Container(int* colors)
{
    bool reached_liquid_f = 0;
    for (size_t i{0}; i < 4; i++)
    {
        if (colors[3-i] == -1)
        {
            if (reached_liquid_f == 1)
            {
                std::cout << "invalid input for container!" << std::endl;
                throw std::exception();
                break;
            }
            this->Capacity++;
            this->colors[3 - i] = -1;
            continue;
        }
        else
        {
            reached_liquid_f = 1;
            this->colors[3-i] = colors[3-i];
        }
    }
}

Container::Container(const Container& source)
{
    this->Capacity = source.Capacity;
    for (size_t i{}; i < 4; i++)
    {
        this->colors[i] = source.colors[i];
    }
}

size_t Container::get_capacity()
{
    return Capacity;
}

int Container::get_colors(size_t i)
{
    return this->colors[i];
}

void Container::pour_by(Container& source)
{
    if (this->Capacity > 0 && source.Capacity != 4 && this != &source)
    {
        for (int i{3}; i >= 0; i--)
        {
            if(source.colors[i] == -1)
                continue;
            if (this->Capacity == 4)
            {
                this->colors[0] = source.colors[i];
                source.colors[i] = -1;
                this->Capacity--;
                source.Capacity++;
                continue;
            }
            if(this->Capacity > 0 && this->colors[3-this->Capacity] == source.colors[i])
            {
                this->colors[4 - this->Capacity] = source.colors[i];
                source.colors[i] = -1;
                this->Capacity--;
                source.Capacity++;
            }
            else
                break;
        }
    }
}

bool Container::is_pourable_by(Container& source)
{
    if (this->Capacity > 0 && source.Capacity != 4 && this != &source)
    {
        for (int i{3}; i >= 0; i--)
        {
            if(source.colors[i] == -1)
                continue;
            if (this->Capacity == 4)
            {
                return true;
            }
            if(this->Capacity > 0 && this->colors[3-this->Capacity] == source.colors[i])
            {
                return true;
            }
            else
                return false;
        }
        return false;
    }
    return false;
}

bool Container::is_fool_pour(Container& source)
{
    bool is_fool_flag{true};
    if (this->Capacity == 1 && source.Capacity == 1 && source.colors[2] == source.colors[3])
        return true;
    
    // is_fool_flag = true;
    // if (this->Capacity < source.Capacity)
    // {
    //     int temp = source.colors[0];
    //     for (size_t i{}; i < 4 - source.Capacity; i++)
    //     {
    //         if(temp != source.colors[i])
    //         {
    //             is_fool_flag = false;
    //             break;
    //         }
    //     }
    //     if (is_fool_flag)
    //         return true;
    // }

    is_fool_flag = true;
    if(this->Capacity != 4)
    {
        is_fool_flag = false;
    }
    else
    {
        int temp = source.colors[0];
        for (size_t i{}; i < 4 - source.Capacity; i++)
        {
            if(temp != source.colors[i])
            {
                is_fool_flag = false;
                break;
            }
        }
        if (is_fool_flag)
            return true;
    }

    if (this->Capacity == 1 && source.colors[2] == this->colors[2])
    {
        return true;
    }
    if (this->Capacity == 2 && source.colors[1] == this->colors[1])
    {
        return true;
    }

    return is_fool_flag;
}

void Container::show()
{
    for (size_t i{0}; i < 4; i++)
        if(this->colors[i] == -1)
            std::cout << "[ ]";
        else
            std::cout << "[" << this->colors[i] << "]";
    // std::cout << "C = " << this->Capacity << std::endl;
    std::cout << std::endl;
}

bool Container::is_full_of_one_color()
{
    for (size_t i{}; i < 3; i++)
    {
        if (this->colors[i] != this->colors[i + 1])
            return false;
    }
    return true;
}