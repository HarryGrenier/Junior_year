def load_csv_data(url):
    import pandas as pd
    return pd.read_csv(url)

def load_json_data(url):
    import pandas as pd
    return pd.read_json(url)

dataset_list = [{'url': 'https://raw.githubusercontent.com/rfordatascience/tidytuesday/master/data/2022/2022-05-03/capacity.csv',
     'name': 'capacity',
     'load_func': load_csv_data},
     {'url': 'https://raw.githubusercontent.com/rfordatascience/tidytuesday/master/data/2021/2021-03-16/games.csv',
     'name': 'games',
     'load_func': load_csv_data},
     {'url': 'https://raw.githubusercontent.com/prust/wikipedia-movie-data/master/movies.json' , #any dataset you're interested in
     'name': 'American movies',
     'load_func': load_json_data}]