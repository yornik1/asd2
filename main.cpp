#include <iostream>
#include <string.h>
#include <list>
#include <iomanip>
#include <fstream>

struct Student{
    std::string lastname;
    int day;
    int month;
    int year;
    float *scores;
    int course;
    Student *prev, *next;
};

Student *head=0;
Student *tail=0;

void insert(std::string lastname, int day, int month, int year, float *scores, int course, int pos) {
    Student *newItem = new Student;
    newItem->lastname = lastname;
    newItem->day = day;
    newItem->month = month;
    newItem->year = year;
    newItem->scores = scores;
    newItem->course = course;
    newItem->prev = 0;
    newItem->next = 0;
    if (head == 0 && pos == 1) /* если необходимо добавить элемент в первую позицию пустого списка */
    {
        head = newItem; //новый элемент объвляем головой
        tail = head;
    } else {
        Student *current = head; //находим в списке элемент, после которого будем вставлять новый
        for (int i = 1; i < pos && current != 0; i++) current = current->next;
        if (current == 0) {
            tail->next = newItem;
            newItem->prev = tail;
            tail = newItem;
        } else {
            if (pos == 1) { /* если вставляем в первую позицию непустого списка */

                /* текущая голова списка становится следующим элементом для вставляемого */
                newItem->next = head;
                head->prev = newItem;
                head = newItem; //новый элемент становится головой списка
            } else {
                if (current->next != 0) //если не последний элемент
                    current->next->prev = newItem; /* объявляем добавляемый предыдущим для pos+1-го */
                newItem->next = current->next; // связываем добавляемый элемент и pos-ный адресными указателями
                current->next = newItem;
                newItem->prev = current;
            }
        }
    }
};

void del(int pos){
    Student *tmp; //вспомогательный указатель
    Student *current = head;
    for (int i = 1; i < pos && current != 0; i++){
        tmp=current;
        current = current->next;
    }
    /* При выходе из цикла в current храним адрес удаляемого элемента, а в tmp адрес элемента, ему предшествующего*/
    if (current != 0){ //если элемент с номером k есть в спи
        if (current == head) //если удаляется первый элемент
            head = head->next; //второй станет первым
        else //если удаляем не первый элемент
            tmp->next = current->next; /* связываем предыдущий и следующий элемент списка, исключая таким образом,
 * из цепочки связи
                                        удаляемый элемент*/
        delete current; //непосредственно удаляем элемент из памяти
    }
};

void show_students(){
    Student *current=head;
    std::cout<<"\nlastname      |birthdate\t|scores\t\t\t\t\t |course"<<std::endl;
    while(current!=0){
        std::cout<<std::setw(20)<<std::left<<current->lastname<<"|";
        std::cout<<current->day;
        std::cout<<":"<<current->month;
        std::cout<<":"<<current->year<<"\t|";
        for (int i=0; i<4; i++) std::cout<<current->scores[i]<<"  ";
        std::cout<<"\t\t |"<<current->course;
        std::cout<<"\n";
        current=current->next;
    }
}


void score_higher_students(int score){
    Student *current=head;
    Student *tmp;
    bool bNotSorted=true, bSwapped;
    // фильтр по оценкам
    int pos = 0;
    while(current){
        pos++;
        if((current->scores[0] + current->scores[1] + current->scores[2] + current->scores[3]) < score) {
            del(pos);
            current=current->next;
            continue;
        }
        current=current->next;
    }
    // сортировка списка студентов по алфавиту
    while(bNotSorted){ //пока находим неотсортированные элементы
        bNotSorted=false;
        current=head; //идем от начала списка

        while(current->next) //перебираем элементы до конца списка
        {
            bSwapped=false;
            if(current->lastname > current->next->lastname){
                bNotSorted=true;
                bSwapped=true;
                if(current!=head)
                    current->prev->next=current->next; //1
                else
                    head=current->next;
                current->next->prev=current->prev; //2
                tmp=current->next->next;
                current->next->next=current; //3
                current->prev=current->next;//4
                current->next=tmp; //5
                if(current->next==0)
                    tail=current;
                else
                    current->next->prev=current; //6
            }
            if(!bSwapped)
                current=current->next;
        }
    }
};

int main() {
    int n;
    int rating=18;
    int to_delete;
    std::string lastname;
    int day=12, month=12, year=1991, course=3;
    std::string lastnames[5] = {"иванов","петров","попова","бобров","марков"};
    printf("Enter students count\n");
//    std::cin>>n;
    n = 5;
    float scores[5][4]= {{3.9,4.1,3.4,4.5},
                         {4.1,4.2,5.0,4.9},
                         {4.1,4.2,5.0,4.9},
                         {4.1,4.2,5.0,4.9},
                         {4.1,4.2,3.9,4.1}};
//    setlocale(LC_ALL, "rus");
//    char buff[50]; // буфер промежуточного хранения считываемого из файла текста
//    std::ifstream fin("data.in"); // открыли файл для чтения
//
//    fin >> buff; // считали первое слово из файла
//    std::cout << buff << std::endl; // напечатали это слово
//
//    fin.getline(buff, 50); // считали строку из файла
//    fin.close(); // закрываем файл
//    std::cout << buff << std::endl; // напечатали эту строку

    printf("Enter students's data: lastname birthday month year scores(4 floats) course-number\n");
    for(int i=0; i<n; i++){
        std::cin>>lastname;
        std::cin>>day;
        std::cin>>month;
        std::cin>>year;
        for (int j=0; j<4; j++){
            std::cin>>scores[i][j];
        };
        std::cin>>course;
//        lastname = lastnames[i];
        insert(lastname, day, month, year, scores[i], course, i+1);
        printf("Student inserted\n");
    }
    printf("\nAll students");
    show_students();
    printf("\nStudent's no. to delete (1, %d)", n);
    std::cin>>to_delete;
    del(to_delete);
    printf("\nStudents after deleting");
    show_students();
    printf("Enter students rating\n");
    std::cin>>rating;
    score_higher_students(rating);
    printf("\nStudents with score more than %d", rating);
    show_students();
    return 0;
}
