import functools
from random import Random, sample

PANGRAM_FILE = 'resources/pangrams.txt'
SHORT_WORD_FILE = 'resources/short_words.txt'

static_value_map = None

def value_generator(func):
    @functools.wraps(func)
    def decorated_func(*args, **kwargs):
        varname = None
        if 'varname' in kwargs:
            varname = kwargs.pop('varname')

        if static_value_map is None:
            value = func(*args, **kwargs)

        else:
            value = static_value_map[varname]

        # if varname is not None:
        #     print("Generated value {} = {}".format(varname, value))

        return value
    return decorated_func


@value_generator
def simple_int_range(min, max, local_random = None):
    if local_random is None:
        local_random = Random()
    return local_random.randint(min, max)

@value_generator
def pangram_generate():
    open_file = open(PANGRAM_FILE, 'r').read()
    pangram_list = open_file.split("\n")
    
    random_pangram = sample(pangram_list, 1)
    return '"{}"'.format(random_pangram[0])

@value_generator
def array_of_length(length, min, max):
    result = "{"
    for i in range(length):
        result = result + str(simple_int_range(min, max)) + ","
    return result + "}"

@value_generator
def short_strings_gen(count):
    open_file = open(SHORT_WORD_FILE, 'r').read()
    short_words_list = open_file.split("\n")

    index = 0
    random_words = ""
    for i in range(count):
        random_words = random_words + sample(short_words_list, 1)[0]
        index = index + 1
        if (index < count):
            random_words = random_words + " "
    return random_words