#include <iostream>

using namespace std;

class Date
{
    int year_;
    int month_;
    int day_;
public:
    Date(int year = 2000, int month = 1, int day = 1)
    {
        year_ = year;
        month_ = month;
        day_ = day;
    }
    void SetDate(int year = 2000, int month = 1, int date = 1);
    void AddDay(int inc);
    void AddMonth(int inc);
    void AddYear(int inc);

    void ShowDate();
};

int main(void)
{
    Date date;

    date.SetDate();

    char input;

    cout << "초기 설정 값을 지정하시겠습니까(y/N) : ";
    cin >> input;

    if(input == 'y' || input == 'Y')
    {
        int y, m, d;
        cout << "yyyy/mm/dd/에 들어갈 년도와 달 그리고 일을 입력해주세요 : ";
        cin >> y >> m >> d;
        date.SetDate(y, m, d);
        date.ShowDate();
    }
    
    for(;;)
    {
        int choose;
        cout << endl;
        cout << "1. 년도 증가" << endl;
        cout << "2. 월 증가" << endl;
        cout << "3. 날짜 증가" << endl;
        cout << "증가시킬 항목을 선택해주세요 : ";
        cin >> choose;

        switch(choose)
        {
            int input_num;
            case 1: cout << "몇 년 증가시키시겠습니까 : ";
                    cin >> input_num;
                    date.AddYear(input_num);
                    date.ShowDate();
                    break;
            case 2: cout << "몇 달 증가시키시겠습니까 :";
                    cin >> input_num;
                    date.AddMonth(input_num);
                    date.ShowDate();
                    break;
            case 3: cout << "몇 일 증가시키시겠습니까 : ";
                    cin >> input_num;
                    date.AddDay(input_num);
                    date.ShowDate();
                    break;
            default: cout << endl;
                     cout << "목록에 있는 1, 2, 3 중에 선택해주세요." << endl;
        }
    }

    return 0;
}

void Date::SetDate(int year, int month, int day) //년도, 월, 날짜 초기화
{
    year_ = year;
    month_ = month;
    day_ = day;
 
    cout << "초기 설정된 날짜는 " << this->year_ << "/" << this->month_ << "/" << this->day_ << " 입니다." << endl;
}

void Date::AddDay(int inc) // 날짜 증가
{
    if(month_ >= 1 && month_ <= 7) // 1월부터 7월까지
    {
        day_ += inc;

        if(month_ % 2 == 1) // 홀수 월
        {
            if(day_ > 31)
            {
                month_++;
                day_ -= 31;
            }
        }

        else if(month_ % 2 == 0) // 짝수 월
        {
            if(month_ == 2) //단, 2월일 때는 따로 처리
            {
                if((year_ % 4 == 0) && (year_ % 100 != 0) || (year_ % 400 == 0)) // 2월 마지막날이 29일
                {
                    if(day_ > 29)
                    {
                        month_++;
                        day_ -= 29;
                    }
                }
                else
                {
                    if(day_ > 28)
                    {
                        month_++;
                        day_ -= 28;
                    }
                }
            }
            
            if(day_ > 30)
            {
                month_++;
                day_ -= 30;
            }
        }
    }

    else if(month_ >= 8 && month_ <= 12) // 8월부터 12월까지
    {
        day_ += inc;

        if(month_ % 2 == 0) // 짝수 월
        { 
            if(day_ > 31)
            {
                month_++;
                if(month_ > 12)
                {
                    year_++;
                    month_ -= 12;
                    day_ -= 31;
                }
                else
                    day_ -= 31;
            }
        }

        else if(month_ % 2 == 1) // 홀수 월
        {
            if(day_ > 30)
            {
                month_++;
                day_ -= 30;
            }
        }
    }
}

void Date::AddMonth(int inc) // 월 증가
{
    int add_month_inc = month_ + inc;

    if(add_month_inc >= 1 && add_month_inc <= 7) // 1월부터 7월까지
    {
        if(add_month_inc % 2 == 1) // month 값과 입력 값의 합이 홀수 월일 때
        {
            if(day_ > 31)
            {
                month_ += 2;
                day_ -= 31;
            }
            else
                month_ += inc;
        }

        else if(add_month_inc % 2 == 0) // month 값과 입력 값의 합이 짝수 월일 때
        {
            if(add_month_inc == 2) // 단, 2월은 따로 처리
            {
                if((year_ % 4 == 0) && (year_ % 100 != 0) || (year_% 400 == 0)) // 2월 중 마지막 날이 29일
                {
                    if(day_ > 29)
                    {
                        month_ += 2;
                        day_ -= 29;
                    }
                    else
                        month_ += inc;
                }
                else
                {
                    if(day_ > 28)
                    {
                        month_ += 2;
                        day_ -= 28;
                    }
                    else
                        month_ += inc;
                }
            }

            if(day_ > 30)
            {
                month_ += 2;
                day_ -= 30;
            }
        }
    }

    else if(add_month_inc >= 8 && add_month_inc <= 12) // 8월부터 12월까지
    {
        if(add_month_inc % 2 == 0) // 짝수 월
        {
            if(day_ > 31)
            {
                month_ += 2;
                day_ -= 31;
            }
            else
                month_ += inc;
        }

        else if(add_month_inc % 2 == 1) // 홀수 월
        {
            if(day_ > 30)
            {
                month_ += 2;
                day_ -= 30;
            }
            else
                month_ += inc;
        }
    }

    /*
        else
        month_ += inc; // 출력으로 쓰일 변수에 실제 값 저장
    */

    if(month_ > 12) 
    {
            year_++;
            month_ -= 12;
            day_ -= 31;
    }
}

void Date::AddYear(int inc)
{
    year_ += inc;
}

void Date::ShowDate()
{
    cout << endl;
    cout << "----------------------------------" << endl;
    cout << "| " << "증가한 날짜는" << this->year_ << "/" << this->month_ << "/" <<this->day_ << " 입니다." << " |" << endl;
    cout << "----------------------------------" << endl;
}
