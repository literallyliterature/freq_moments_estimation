def estimate(d, k):
  return sum((i**k for i in d.values()))

def process(filename):
  d = {}

  with open('./distributions/' + filename) as f:
    if ('.dat' in filename):
      for line in f.readlines():
        for item in line.split():
          d[item] = d.get(item, 0) + 1
    else:
      for line in f.readlines():
        item, v = line.split(',')
        d[item] = d.get(item, 0) + int(v)


  result = [filename] + [str(estimate(d, i)) for i in range(11)]
  print(',').join(result)


if __name__ == '__main__':
  csv_header = ['filename'] + ['estimate(' + str(i) + ')' for i in range(11)]
  print ','.join(csv_header)

  filenames = [
    "uniform_100.txt",
    "uniform_1k.txt",
    "uniform_10k.txt",
    "uniform_100k.txt",
    "uniform_1M.txt",
    "uniform_10M.txt",
    "normal_0.05_10k.txt",
    "normal_0.05_100k.txt",
    "normal_0.05_1M.txt",
    "normal_0.1_10k.txt",
    "normal_0.1_100k.txt",
    "normal_0.1_1M.txt",
    "normal_0.25_10k.txt",
    "normal_0.25_100k.txt",
    "normal_0.25_1M.txt",
    "normal_0.5_10k.txt",
    "normal_0.5_100k.txt",
    "normal_0.5_1M.txt",
    "kosarak.dat",
    "Webdocs.dat"
  ]
  for filename in filenames:
    process(filename)
