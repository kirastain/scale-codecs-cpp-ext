import re

data = str = open("/Users/kirastain/Documents/rob/scale-codecs-cpp-ext/json/metadata_3.json", 'r').read()
formatted = re.sub("(\w+):", r'"\1":',  data)

res_file = open("metadata_3_formatted.json", "w")
res_file.write(formatted)
res_file.close()