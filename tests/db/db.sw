SQLite format 3   @                                                                    -�   	�    ���������                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	            � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             7  �?PbM��� @  @           
  mutations_probability REAL NOT NULL,  -- Values from 0 to 1
  time_birth INTEGER NOT NULL,
  time_mutate INTEGER NOT NULL,

  time_laziness INTEGER NOT NULL,
  energy_laziness INTEGER NOT NULL,

  attack_multiplier REAL NOT NULL,

  energy_nothing INTEGER NOT NULL,
  energy_myself INTEGER NOT NULL,
  energy_detect INTEGER NOT NULL,
  energy_info INTEGER NOT NULL,
  energy_move INTEGER NOT NULL,
  energy_turn INTEGER NOT NULL,
  energy_attack INTEGER NOT NULL,
  energy_eat INTEGER NOT NULL,
  energy_egg INTEGER NOT NULL,

  PRIMARY KEY(id),
  CHECK(time >= 0),
  CHECK(size_x > 0 AND size_y > 0),
  CHECK(mutations_probability >= 0 AND mutations_probability <= 1),
  CHECK(time_birth >= 0),
  CHECK(time_mutate >= 0),
  CHECK(time_laziness >= 0),
  CHECK(energy_laziness >= 0),
  CHECK(energy_nothing >= 0 AND energy_myself >= 0 AND
        energy_detect >= 0 AND energy_info >= 0 AND
        energy_move >= 0 AND energy_turn >= 0 AND
        energy_attack >= 0 AND energy_eat >= 0 AND
        energy_egg >= 0)
)   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ���4Vx�  +<M^ox�  4Vx����   � �i�                                                                                                                                                                                                                                                                                                                                                                                                   �M##�atableEnvironmentEnvironmentCREATE TABLE Environment
(
  id INTEGER NOT NULL,

  time INTEGER NOT NULL,

  -- the rest of the rows shouldn't change
  size_x INTEGER NOT NULL,
  size_y INTEGER NOT NULL,
   �i�1tableWorldWorldCREATE TABLE World
(
  id INTEGER NOT NULL,

  position_x INTEGER NOT NULL,
  position_y INTEGER NOT NULL,
  orientation INTEGER,

  PRIMARY KEY(id),
  UNIQUE(position_x, position_y),
  CHECK(position_x >= 0 AND position_y >= 0),
  CHECK(orientation IS NULL OR
        (orientation >= 0 AND orientation <= 3)) -- 4 possible orientations
))= indexsqlite_autoindex_World_1World    �  �B�                                                                                                                                                                             ��otableBugBugCREATE TABLE Bug
(
  id INTEGER NOT NULL,

  father_id INTEGER,                    -- NULL if the bug is added manually

  /* The blob is the last col for performance */
  code BLOB NOT NULL,

  PRIMARY KEY(id),
  FOREIGN KEY(father_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE SET NULL,
  CHECK(father_id),
  CHECK(length(code) >= 0 AND (length(code) % 4 = 0))
)��tableEggEgg	CREATE TABLE Egg
(
  bug_id INTEGER NOT NULL,

  world_id INTEGER NOT NULL,
  energy INTEGER NOT NULL,
  conception INTEGER NOT NULL,

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,
  UNIQUE(world_id),
  CHECK(energy > 0),
  CHECK(conception >= 0)
)%9 indexsqlite_autoindex_Egg_1Egg
   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           	 d �
   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ,K~ � 	

   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    �  �                                                                                                                                                                                                                         ��tableAliveBugAliveBugCREATE TABLE AliveBug
(
  bug_id INTEGER NOT NULL,

  world_id INTEGER NOT NULL,
  birth INTEGER NOT NULL,
  energy INTEGER NOT NULL,

  time_last_action INTEGER,             -- NULL if no action was done yet
  action_time INTEGER,                  -- NULL if the bug is not doing a
                                        -- action

  /* The blob is the last col for performance */
  registers BLOB NOT NULL,

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,
  UNIQUE(world_id),
  CHECK(time_last_action IS NULL OR time_last_action >= 0),
  CHECK(action_time IS NULL OR action_time > 0),
  CHECK(birth >= 0),
  CHECK(energy > 0)
)   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           	 d �   � ��                                                                                                                                                                                                                                                                                                                                                                                                                       /C indexsqlite_autoindex_AliveBug_1AliveBug�)	�)tableDeadBugDeadBugCREATE TABLE DeadBug
(
  bug_id INTEGER NOT NULL,

  birth INTEGER,                        -- NULL if it was a egg when it died
  death INTEGER NOT NULL,

  killer_id INTEGER,                    -- NULL if the bug wasted his energy

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(killer_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE SET NULL,
  CHECK(birth IS NULL OR birth >= 0),
  CHECK(birth IS NULL OR birth <= death),
  CHECK(bug_id <> killer_id)
)   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          �   �  �     E  Erz�                                                     �*
�'tableMutationMutationCREATE TABLE Mutation
(
  id INTEGER NOT NULL,

  bug_id INTEGER NOT NULL,

  time INTEGER NOT NULL,

  position INTEGER NOT NULL,
  /* original and mutated can't be NULL at same time */
  original INTEGER,                     -- NULL if new code was added
  mutated INTEGER,                      -- NULL if code was deleted

  PRIMARY KEY(id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  CHECK(time >= 0),
  CHECK(position >= 0),
  CHECK((original IS NOT NULL) OR (mutated IS NOT NULL))
)��mtableFoodFoodCREATE TABLE Food
(
  id INTEGER NOT NULL,

  world_id INTEGER NOT NULL,
  size INTEGER NOT NULL,

  PRIMARY KEY(id),
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,
  UNIQUE(world_id),
  CHECK(size >= 0)
)'; indexsqlite_autoindex_Food_1Food[/#sindexEnvironment_indexEnvironmentCREATE INDEX Environment_index ON Environment(time)   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                �    �  ���                                                                                                                                �~;#�'triggerEnvironment_insert_timeEnvironment CREATE TRIGGER Environment_insert_time
BEFORE INSERT
ON Environment
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'There is a older time')
  WHERE (SELECT max(time)
         FROM Environment) > NEW.time;
END�~;#�'triggerEnvironment_update_timeEnvironment CREATE TRIGGER Environment_update_time
BEFORE UPDATE
ON Environment
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'There is a older time')
  WHERE (SELECT max(time)
         FROM Environment) > NEW.time;
END�m1#�triggerEnvironment_insertEnvironment CREATE TRIGGER Environment_insert
BEFORE INSERT
ON Environment
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The size of the World can''t change')
  WHERE (SELECT count(id)
         FROM Environment) > 0
	AND
        (SELECT id
         FROM Environment
         WHERE size_x=NEW.size_x AND size_y=NEW.size_y) IS NULL;
END   b b�                                                                                                                                                                                                                                                                                                                                                      �L;�OtriggerWorld_insert_position_xWorld CREATE TRIGGER World_insert_position_x
BEFORE INSERT
ON World
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_x;
END�L;�OtriggerWorld_insert_position_yWorld CREATE TRIGGER World_insert_position_y
BEFORE INSERT
ON World
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_x;
END    �  �I�                                                                                                                                                                                                      �r;�triggerWorld_update_position_xWorld CREATE TRIGGER World_update_position_x
BEFORE UPDATE
ON World
FOR EACH ROW
WHEN OLD.position_x <> NEW.position_x
BEGIN
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_x;
END�r;�triggerWorld_update_position_yWorld CREATE TRIGGER World_update_position_y
BEFORE UPDATE
ON World
FOR EACH ROW
WHEN OLD.position_y <> NEW.position_y
BEGIN
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_x;
END@]indexBug_indexBugCREATE INDEX Bug_index ON Bug(father_id)
   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        x  xl�                                                                                                          �q7�!triggerEgg_insert_conceptionEgg CREATE TRIGGER Egg_insert_conception
BEFORE INSERT
ON Egg
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'conception is not now')
  WHERE (SELECT max(time)
         FROM Environment) <> NEW.conception;
END�.M�{triggerAliveBug_insert_time_last_actionAliveBug CREATE TRIGGER AliveBug_insert_time_last_action
BEFORE INSERT
ON AliveBug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The time_last_action is in the future')
  WHERE (SELECT max(time)
         FROM Environment)
        < NEW.time_last_action;
END�`M�_triggerAliveBug_update_time_last_actionAliveBug CREATE TRIGGER AliveBug_update_time_last_action
BEFORE UPDATE
ON AliveBug
FOR EACH ROW
WHEN OLD.time_last_action <> NEW.time_last_action
BEGIN
  SELECT RAISE(ROLLBACK, 'The time_last_action is in the future')
  WHERE (SELECT max(time)
         FROM Environment)
        < NEW.time_last_action;
END    � ��P �                                                                                                                                                                                                                                             Q)kindexMutation_indexMutationCREATE INDEX Mutation_index ON Mutation(bug_id)P'mindexDeadBug_indexDeadBugCREATE INDEX DeadBug_index ON DeadBug(killer_id)�@C�)triggerAliveBug_update_action_timeAliveBug CREATE TRIGGER AliveBug_update_action_time
BEFORE UPDATE
ON AliveBug
FOR EACH ROW
WHEN OLD.action_time <> NEW.action_time
BEGIN
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')
  WHERE (SELECT max(time)
         FROM Environment)
        > NEW.action_time;
END�C�YtriggerAliveBug_insert_action_timeAliveBug CREATE TRIGGER AliveBug_insert_action_time
BEFORE INSERT
ON AliveBug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')
  WHERE (SELECT max(time)
         FROM Environment)
        > NEW.action_time;
END
   � �                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
   � ���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                