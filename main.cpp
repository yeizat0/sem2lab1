//5.Створіть двобічно зв'язаний список ноутбуків. Програма має меню, можливо додавати, видаляти,
//шукати ноутбуки, можна виконувати ці операції за індексом. Існує можливість запису поточного
//списку у файл, а також можливість завантажити список у програму за введеним ім'ям файлу.

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

struct Laptop {
    std::string name;
    int cpu;
    int ram;
    int storage;
};

Laptop LaptopNew(std::string name, int cpu, int ram, int storage) {
    return {.name = std::move(name), .cpu = cpu, .ram = ram, .storage = storage};
}

void LaptopPrint(const Laptop &laptop) {
    std::cout << "Laptop info: " << std::endl;
    std::cout << "Name: " << laptop.name << std::endl;
    std::cout << "CPU: " << laptop.cpu << std::endl;
    std::cout << "RAM: " << laptop.ram << std::endl;
    std::cout << "Storage: " << laptop.storage << std::endl;
    std::cout << "*********************************" << std::endl;
}


struct Node {
    Laptop laptop;
    Node *prev;
    Node *next;
};

Node *NodeNew(const Laptop &laptop) {
    return new Node{.laptop = laptop, .prev = nullptr, .next = nullptr};
}


struct List {
    Node *head;
    Node *tail;
    size_t size;
};

Node *ListGetAt(const List &list, size_t index);
void ListFindLaptop(const List &list, const std::string &laptop_name);
void ListPrint(const List &list);
void ListPopBack(List *list);
void ListPopFront(List *list);
void ListDeleteAt(List *list, size_t index);
void ListPushFront(List *list, const Laptop &laptop);
void ListPushBack(List *list, const Laptop &laptop);
void ListInsertAt(List *list, const Laptop &laptop, size_t index);
void ListWriteFile(const List &list, const std::string &filename);
List ListReadFile(const std::string &filename);
Laptop LaptopCreate();
void ListDelete(List *list);

void ListInsertAt(List *list, const Laptop &laptop, size_t index) {
    if (!list) {
        return;
    }
    Node *node = NodeNew(laptop);
    if (!list->head || !list->tail) {
        list->head = node;
        list->tail = node;
        list->size++;
        return;
    }
    if (index == 0) {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
        list->size++;
        return;
    }
    if (index >= list->size) {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
        list->size++;
        return;
    }

    Node *curr = ListGetAt(*list, index);
    node->next = curr;
    node->prev = curr->prev;
    curr->prev->next = node;
    curr->prev = node;
    list->size++;
}

void ListPushBack(List *list, const Laptop &laptop) {
    ListInsertAt(list, laptop, list->size);
}

void ListPushFront(List *list, const Laptop &laptop) {
    ListInsertAt(list, laptop, 0);
}

void ListDeleteAt(List *list, size_t index) {
    if (!list) {
        return;
    }
    if (!list->head || !list->tail || list->size == 0) {
        return;
    }
    if (list->size == 1) {
        delete list->head;
        list->head = nullptr;
        list->tail = nullptr;
        list->size = 0;
        return;
    }
    if (index == 0) {
        list->head = list->head->next;
        delete list->head->prev;
        list->head->prev = nullptr;
        list->size--;
        return;
    }
    if (index >= list->size) {
        list->tail = list->tail->prev;
        delete list->tail->next;
        list->tail->next = nullptr;
        list->size--;
        return;
    }
    Node *curr = ListGetAt(*list, index);
    Node *prev = curr->prev;
    Node *next = curr->next;
    delete curr;
    prev->next = next;
    next->prev = prev;
    list->size--;
}

void ListPopFront(List *list) {
    ListDeleteAt(list, 0);
}

void ListPopBack(List *list) {
    ListDeleteAt(list, list->size);
}

void ListPrint(const List &list) {
    for (const Node *current = list.head; current; current = current->next) {
        LaptopPrint(current->laptop);
    }
}

void ListFindLaptop(const List &list, const std::string &laptop_name) {
    bool found = false;
    for (const Node *current = list.head; current; current = current->next) {
        if (current->laptop.name == laptop_name) {
            LaptopPrint(current->laptop);
            found = true;
        }
    }
    if (!found) {
        std::cout << "No such laptop found!" << std::endl;
    }
}

Node *ListGetAt(const List &list, const size_t index) {
    if (index >= list.size) {
        return nullptr;
    }
    Node *current = list.head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

void ListWriteFile(const List &list, const std::string &filename) {
    std::ofstream laptops(filename);
    for (const Node *current = list.head; current; current = current->next) {
        laptops << "Name: " << current->laptop.name << std::endl;
        laptops << "CPU: " << current->laptop.cpu << std::endl;
        laptops << "RAM: " << current->laptop.ram << std::endl;
        laptops << "Storage: " << current->laptop.storage << std::endl;
    }
}

List ListReadFile(const std::string &filename) {
    List list = {};
    std::ifstream laptops(filename);
    if (!laptops.is_open()) {
        std::cout << "Could not open file: " << filename << std::endl;
        return list;
    }
    std::string name;
    while (std::getline(laptops, name)) {
        Laptop laptop = {};
        size_t name_space = name.find(' ');
        std::string laptop_name = name.substr(name_space + 1, name.length());
        laptop.name = laptop_name;

        std::string cpu;
        std::getline(laptops, cpu);
        size_t cpu_space = cpu.find(' ');
        std::string laptop_cpu = cpu.substr(cpu_space, cpu.length());
        int cpu_num = std::stoi(laptop_cpu);
        laptop.cpu = cpu_num;

        std::string ram;
        std::getline(laptops, ram);
        size_t ram_space = ram.find(' ');
        std::string laptop_ram = ram.substr(ram_space, ram.length());
        int ram_num = std::stoi(laptop_ram);
        laptop.ram = ram_num;

        std::string storage;
        std::getline(laptops, storage);
        size_t storage_space = storage.find(' ');
        std::string laptop_storage = storage.substr(storage_space, storage.length());
        int storage_num = std::stoi(laptop_storage);
        laptop.storage = storage_num;

        ListPushBack(&list, laptop);
    }
    return list;
}

Laptop LaptopCreate() {
    std::cout << "Enter laptop specs:" << std::endl;

    std::cout << "Name: " << std::endl;
    std::string name;
    std::cin >> name;

    std::cout << "Number of CPUs: " << std::endl;
    int cpu;
    std::cin >> cpu;

    std::cout << "ram: " << std::endl;
    int ram;
    std::cin >> ram;

    std::cout << "Storage: " << std::endl;
    int storage;
    std::cin >> storage;
    return LaptopNew(name, cpu, ram, storage);
}

void ListDelete(List *list) {
    std::cout << "Enter index of laptop to be deleted:" << std::endl;
    int index = 0;
    std::cin >> index;
    if (index < 0) {
        std::cout << "Index must not be negative!" << std::endl;
        return;
    }
    ListDeleteAt(list, index);
}

void ListSearch(const List &list) {
    std::cout << "Enter name of laptop:" << std::endl;
    std::string laptop_name;
    std::cin >> laptop_name;
    ListFindLaptop(list, laptop_name);
}

List ListRead() {
    std::cout << "Enter name of file:" << std::endl;
    std::string filename;
    std::cin >> filename;
    return ListReadFile(filename);
}

void PrintMenu() {
    std::cout << "Menu:" << std::endl;
    std::cout << "type 1 to create new laptop" << std::endl;
    std::cout << "type 2 to delete laptop" << std::endl;
    std::cout << "type 3 to search laptop" << std::endl;
    std::cout << "type 4 to write list to file" << std::endl;
    std::cout << "type 5 to read list from file" << std::endl;
}

void ListInsert(List *list) {
    std::cout << "Enter index of new laptop:" << std::endl;
    int index = 0;
    std::cin >> index;
    if (index < 0) {
        std::cout << "Index must not be negative:" << std::endl;
        return;
    }

    Laptop laptop = LaptopCreate();
    ListInsertAt(list, laptop, index);
}

int main() {
    const Laptop laptops[] = {
        LaptopNew("Laptop1", 1, 8, 24),
        LaptopNew("Laptop2", 2, 16, 1000),
        LaptopNew("Laptop3", 4, 32, 500),
        LaptopNew("Laptop4", 16, 64, 2000),
        LaptopNew("Laptop5", 32, 288, 20),
    };

    List list = {};
    for (const Laptop &laptop: laptops) {
        ListPushBack(&list, laptop);
    }


    PrintMenu();
    int control = 0;

    while (std::cin >> control) {
        switch (control) {
            case 1: ListInsert(&list);
                break;
            case 2: ListDelete(&list);
                break;
            case 3: ListSearch(list);
                break;
            case 4: ListWriteFile(list, "../laptops.txt");
                break;
            case 5: list = ListRead();
                break;
            default: ;
        }
        PrintMenu();
    }
}
