import csv
import matplotlib.pyplot as plt
import os
import calendar  # 캘린더 모듈 추가
from collections import defaultdict

# CSV 파일 경로
csv_file = r"-"  # 데이터 파일 경로

# 그래프 저장 경로
output_dir = r"-"
output_file = os.path.join(output_dir, "monthly_trash_chart.png")

# 출력 디렉토리 확인
if not os.path.exists(output_dir):
    os.makimport csv
import os
import time
from datetime import datetime
import matplotlib.pyplot as plt
import calendar
from collections import defaultdict

# CSV 파일 경로
updata_file = r"-"
data_file = r"-"

# 그래프 저장 경로
output_dir = r"-"
output_file = os.path.join(output_dir, "monthly_trash_chart.png")

# 출력 디렉토리 확인
if not os.path.exists(output_dir):
    os.makedirs(output_dir)  # 디렉토리가 없으면 생성
    print(f"출력 디렉토리를 생성했습니다: {output_dir}")

# 데이터 CSV 초기 설정
if not os.path.exists(data_file):
    with open(data_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["날짜", "LED ON 개수"])  # 헤더 작성
    print(f"데이터 파일이 생성되었습니다: {data_file}")

# update.csv에서 LED ON 개수를 계산하는 함수
def count_led_on(file_path):
    count = 0
    if os.path.exists(file_path):
        with open(file_path, 'r') as file:
            reader = csv.reader(file)
            next(reader, None)  # 헤더 건너뛰기
            for row in reader:
                # "LED ON" 데이터만 카운트
                if len(row) > 0 and row[0].strip().upper() == "LED ON":
                    count += 1
    return count

# data.csv에 LED ON 개수를 추가하는 함수
def update_data(led_count):
    with open(data_file, 'a', newline='') as file:
        writer = csv.writer(file)
        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        writer.writerow([current_time, led_count])
        print(f"{current_time}에 {led_count}개의 LED ON 데이터를 기록했습니다.")

# 10초 동안 파일 변경 감지 및 LED ON 개수 확인
def monitor_led_on(file_path, duration=10):
    print(f"{duration}초 동안 {file_path}에서 LED ON 데이터를 감지합니다...")
    initial_count = count_led_on(file_path)  # 초기 LED ON 개수
    time.sleep(duration)  # 10초 동안 대기
    final_count = count_led_on(file_path)  # 최종 LED ON 개수
    new_led_count = final_count - initial_count
    return max(new_led_count, 0)  # 음수 방지

# 캘린더 및 그래프 생성
def generate_calendar_and_graph():
    # 데이터 읽기
    daily_data = defaultdict(int)  # 날짜별 쓰레기 횟수 저장
    monthly_data = defaultdict(int)  # 월별 쓰레기 횟수 저장

    with open(data_file, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # 헤더 건너뛰기

        for row in reader:
            date, count = row
            year, month, day = map(int, date.split(' ')[0].split('-'))
            daily_data[(year, month, day)] += int(count)
            monthly_data[month] += int(count)

    # 1. 캘린더 형식으로 출력
    def print_calendar(year, month):
        print(f"\n{year}년 {month}월")
        
        # 기본 캘린더 출력
        print(calendar.month(year, month))

        # 캘린더에 쓰레기 횟수 추가
        cal = calendar.monthcalendar(year, month)
        print("쓰레기 버린 횟수 캘린더")
        for week in cal:
            week_with_data = [
                f"{day}({daily_data.get((year, month, day), 0)})" if day != 0 else "   "
                for day in week
            ]
            print(" ".join(week_with_data))

    # 원하는 연도와 월에 대해 캘린더 출력
    for month in range(1, 13):
        print_calendar(2024, month)

    # 데이터 정리
    months = range(1, 13)  # 1월 ~ 12월
    counts = [monthly_data[month] for month in months]

    # 그래프 생성
    plt.bar(months, counts, color='skyblue')
    plt.title('Monthly LED ON Count')
    plt.xlabel('Month')
    plt.ylabel('LED ON Count')
    plt.xticks(months)

    # 그래프 저장 및 확인
    plt.savefig(output_file)  # 그래프를 이미지로 저장
    print(f"그래프가 저장되었습니다: {output_file}")
    plt.show()

# 프로그램 실행
if __name__ == "__main__":
    print("LED ON 데이터 감지 및 최신화를 시작합니다...")
    if os.path.exists(updata_file):
        new_led_count = monitor_led_on(updata_file, duration=10)
        update_data(new_led_count)
        generate_calendar_and_graph()
    else:
        print(f"{updata_file} 파일이 존재하지 않습니다.")
    print("프로그램이 종료됩니다.")
edirs(output_dir)  # 디렉토리가 없으면 생성
    print(f"출력 디렉토리를 생성했습니다: {output_dir}")

# 파일 경로 출력
print(f"확인 중인 CSV 파일 경로: {csv_file}")

# 디렉토리와 파일 구분 확인
if os.path.isdir(csv_file):
    print(f"오류: {csv_file}는 디렉토리입니다!")
    exit()
elif not os.path.isfile(csv_file):
    print(f"오류: {csv_file} 파일이 존재하지 않습니다!")
    exit()
else:
    print(f"{csv_file} 파일이 확인되었습니다.")

# 데이터 읽기
daily_data = defaultdict(int)  # 날짜별 쓰레기 횟수 저장
monthly_data = defaultdict(int)  # 월별 쓰레기 횟수 저장

with open(csv_file, 'r') as file:
    reader = csv.reader(file)
    next(reader)  # 헤더 건너뛰기

    for row in reader:
        date, count = row
        year, month, day = map(int, date.split('-'))
        daily_data[(year, month, day)] += int(count)
        monthly_data[month] += int(count)

# 1. 캘린더 형식으로 출력
def print_calendar(year, month):
    print(f"\n{year}년 {month}월")
    
    # 기본 캘린더 출력
    print(calendar.month(year, month))

    # 캘린더에 쓰레기 횟수 추가
    cal = calendar.monthcalendar(year, month)
    print("쓰레기 버린 횟수 캘린더")
    for week in cal:
        week_with_data = [
            f"{day}({daily_data.get((year, month, day), 0)})" if day != 0 else "   "
            for day in week
        ]
        print(" ".join(week_with_data))

# 원하는 연도와 월에 대해 캘린더 출력
for month in range(1, 13):
    print_calendar(2024, month)

# 데이터 정리
months = range(1, 13)  # 1월 ~ 12월
counts = [monthly_data[month] for month in months]

# 그래프 생성
plt.bar(months, counts, color='skyblue')
plt.title('Monthly Trash Count')
plt.xlabel('Month')
plt.ylabel('Trash Count')
plt.xticks(months)

# 그래프 저장 및 확인
plt.savefig(output_file)  # 그래프를 이미지로 저장
print(f"그래프가 저장되었습니다: {output_file}")
plt.show()
