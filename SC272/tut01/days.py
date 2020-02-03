def day_of_the_week(y,m,d):
    yo = y - (14 - m)//12
    x = yo + yo//4 - yo//100 + yo//400
    mo = m + 12 * ((14 - m)//12) - 2
    return (d + x + (31 * mo)//12) % 7
