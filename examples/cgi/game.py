#!/usr/bin/env python

import os
import urllib.parse
import random
import sys

def update_game_state(game_state, move, player):
    if isinstance(move, str) and move.isdigit():
        move = int(move)
        if move >= 0 and move < len(game_state):
            if game_state[move] == '-':
                game_state = game_state[:move] + player + game_state[move+1:]
    return game_state

def check_winner(game_state):
    winning_conditions = [
        [0, 1, 2], [3, 4, 5], [6, 7, 8],
        [0, 3, 6], [1, 4, 7], [2, 5, 8],
        [0, 4, 8], [2, 4, 6]
    ]
    for condition in winning_conditions:
        if game_state[condition[0]] != '-' and game_state[condition[0]] == game_state[condition[1]] == game_state[condition[2]]:
            return game_state[condition[0]]
    return None

def make_computer_move(game_state):
    available_moves = [i for i, cell in enumerate(game_state) if cell == '-']
    if available_moves:
        return random.choice(available_moves)
    return None

def print_table():
    query_string = os.environ.get('QUERY_STRING', '')
    parsed_data = urllib.parse.parse_qs(query_string)
    game_state = parsed_data.get('game_state', [''])[0]
    move = parsed_data.get('move', [''])[0]

    game_state = game_state[:9].ljust(9, '-')

    num_x = game_state.count('X')
    num_o = game_state.count('O')
    if num_x > num_o:
        player = 'O'
    else:
        player = 'X'

    updated_game_state = update_game_state(game_state, move, player)

    winner = check_winner(updated_game_state)

    if player == 'O' and not winner:
        computer_move = make_computer_move(updated_game_state)
        if computer_move is not None:
            computer_game_state = updated_game_state[:computer_move] + 'O' + updated_game_state[computer_move+1:]
            computer_query_string = urllib.parse.urlencode({'game_state': computer_game_state, 'move': str(computer_move)})
            computer_request_url = os.environ.get('REQUEST_URI', '') + '?' + computer_query_string
            print("<script>window.location.href = '{}';</script>".format(computer_request_url))
            sys.exit(0)

    print("<table>")
    for i in range(3):
        print("<tr>")
        for j in range(3):
            cell_move = i * 3 + j
            if updated_game_state[cell_move] != '-':
                print("<td>{}</td>".format(updated_game_state[cell_move]))
            else:
                cell_game_state = updated_game_state[:cell_move] + player + updated_game_state[cell_move+1:]
                link_body = "game_state={}&move={}".format(cell_game_state, cell_move)
                print("<td><a href='?{}'>{}</a></td>".format(urllib.parse.urlencode({'game_state': cell_game_state, 'move': str(cell_move)}), '-'))
        print("</tr>")
    print("</table>")

    reset_link_body = "game_state=---------&move="
    print("<br>")
    print("<div class='button-container'>")
    print("<form method='GET'>")
    print("<input type='hidden' name='game_state' value='---------'>")
    print("<input type='submit' value='Reset Game'>")
    print("</form>")

    print("<a href='http://localhost:3007/cgi/what_is_cgi.html' class='return-home'>Return</a>")
    print("</div>")

    if winner:
        print("<p>Winner: {}</p>".format(winner))

print("<html>")
print("<head>")
print("<title>Tic-Tac-Toe</title>")
print("<style>")
print(".button-container { display: flex; flex-direction: row; gap: 5vw; }")
print(".button-container form {  margin-right: 3vw; }")
print("table { border-collapse: collapse; }")
print("td { width: 100px; height: 100px; text-align: center; border: 1px solid black; }")
print("</style>")
print("</head>")
print("<body>")
print("<h1>Tic-Tac-Toe</h1>")
print_table()
print("</body>")
print("</html>")
