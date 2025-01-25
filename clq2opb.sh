#!/bin/bash

# 输入文件列表
input_list="/pub/netdisk1/lijy/BHOSLIB/clq_filename.txt"

# 输出目录
output_dir="/pub/netdisk1/lijy/BHOSLIB/opb_format/"

# 遍历输入文件列表
while IFS= read -r clq_file
do
  # 提取文件名（不带路径和扩展名）
  base_name=$(basename "$clq_file" .clq)
  
  # 生成对应的 .opb 文件路径
  opb_file="${output_dir}${base_name}.opb"
  
  # 执行命令
  ./bin/Clq2Opb "$clq_file" "$opb_file" 
  
  echo "Processed $clq_file -> $opb_file"
done < "$input_list"