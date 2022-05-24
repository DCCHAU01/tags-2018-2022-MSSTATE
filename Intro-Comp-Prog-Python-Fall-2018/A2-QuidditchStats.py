#Date Due: Sept. 10, 2018
#Program Description: compiles statistics on a a game called Quidditch from Harry Potter gathered from user input.

#winning team stats
print('Enter the name of the team that caught the Golden Snitch:', end=' ')
gold_snitch_team = input()
print('What was the team\'s final score?', end=' ')
gold_snitch_team_score = input()
snitch = 1

#losing team stats
print('Enter the name of the other team:', end=' ')
opposing_team = input()
print('What was the team\'s final score?', end=' ')
opposing_team_score = input()
print('Enter the length of the game in minutes:', end=' ')
game_length_minutes = input()

#Dividing score of team by length of minutes gives us the goals per minute in the game
goals_per_minute_snitch = float(gold_snitch_team_score) / float(game_length_minutes)
goals_per_minute_other = float(opposing_team_score) / float(game_length_minutes)


#The statistics screen
print('Winning Team\'s Statistics')
print('------------------------')
print('House:' + gold_snitch_team)
print('Goals:' + gold_snitch_team_score)
print('Snitches:' + str(snitch))
print('Goals per Minute:' + str(goals_per_minute_snitch))
print('')

print('Losing Team\'s Statistics')
print('------------------------')
print('House:' + opposing_team)
print('Goals:' + opposing_team_score)
print('Goals per Minute:' + str(goals_per_minute_other))
