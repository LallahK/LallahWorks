def remove_negs(num_list):
    '''Remove negative numbers from the list num_list.'''
    pos_arr = []
    for i in range(len(num_list) - 1, 0, -1):
        if num_list[i] < 0:
            pos_arr.append(num_list[i])
    print(pos_arr)
    for item in pos_arr :
        num_list.remove(item)
