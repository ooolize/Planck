'''
Description: 
Author: lize
Date: 2024-08-29
LastEditors: lize
'''
import matplotlib.pyplot as plt

offset = 20000
second_column = []  # 用于存储第二列的数据
with open('timer.txt', 'r') as file:
    for line in file:
        parts = line.split()  # 按空格分隔
        if len(parts) >= 2:   # 确保有至少两列
            second_column.append(parts)  # 提取第二列并转换为浮点数

# Calculating the deviation from 1,000,000 for the second column for new data
new_offsets = [abs(float(entry[1]) - 1000000) for entry in second_column if abs(float(entry[1]) - 1000000) < offset]

# Plotting the offset values for new data
plt.figure(figsize=(10, 6))
plt.plot(new_offsets, marker='o', linestyle='-', color='r', label='Deviation from 1,000,000')
plt.title('Deviation of Points from 1,000,000 (New Data Set)')
plt.xlabel('Index')
plt.ylabel('Deviation')
plt.grid(True)
plt.legend()
plt.show()
