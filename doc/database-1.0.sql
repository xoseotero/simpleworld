/*******************
 * Environment
 */
CREATE TABLE Environment
(
  id INTEGER NOT NULL,

  time INTEGER NOT NULL,

  -- the rest of the rows shouldn't change
  size_x INTEGER NOT NULL,
  size_y INTEGER NOT NULL,

  time_rot INTEGER NOT NULL,
  size_rot INTEGER NOT NULL,

  mutations_probability REAL NOT NULL,  -- Values from 0 to 1
  time_birth INTEGER NOT NULL,
  time_mutate INTEGER NOT NULL,

  time_laziness INTEGER NOT NULL,
  energy_laziness INTEGER NOT NULL,

  attack_multiplier REAL NOT NULL,

  time_nothing INTEGER NOT NULL,
  time_myself INTEGER NOT NULL,
  time_detect INTEGER NOT NULL,
  time_info INTEGER NOT NULL,
  time_move INTEGER NOT NULL,
  time_turn INTEGER NOT NULL,
  time_attack INTEGER NOT NULL,
  time_eat INTEGER NOT NULL,
  time_egg INTEGER NOT NULL,
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
  CHECK(time_rot >= 0),
  CHECK(size_rot >= 0),
  CHECK(mutations_probability >= 0 AND mutations_probability <= 1),
  CHECK(time_birth >= 0),
  CHECK(time_mutate >= 0),
  CHECK(time_laziness >= 0),
  CHECK(energy_laziness >= 0),
  CHECK(time_nothing >= 0 AND time_myself >= 0 AND
        time_detect >= 0 AND time_info >= 0 AND
        time_move >= 0 AND time_turn >= 0 AND
        time_attack >= 0 AND time_eat >= 0 AND
        time_egg >= 0),
  CHECK(energy_nothing >= 0 AND energy_myself >= 0 AND
        energy_detect >= 0 AND energy_info >= 0 AND
        energy_move >= 0 AND energy_turn >= 0 AND
        energy_attack >= 0 AND energy_eat >= 0 AND
        energy_egg >= 0)
);

CREATE INDEX Environment_index ON Environment(time);

/* time must be the older of the table */
CREATE TRIGGER Environment_insert_time
BEFORE INSERT
ON Environment
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'There is a older time')
  WHERE (SELECT max(time)
         FROM Environment) > NEW.time;
END;

CREATE TRIGGER Environment_update_time
BEFORE UPDATE OF time
ON Environment
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'There is a older time')
  WHERE (SELECT max(time)
         FROM Environment) > NEW.time;
END;

/* The size of the World can't change */
CREATE TRIGGER Environment_insert
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
END;


/*******************
 * World
 */
CREATE TABLE World
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
);

/* positions must be less than the size of the world */
CREATE TRIGGER World_insert
BEFORE INSERT
ON World
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_x;
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_y;
END;

CREATE TRIGGER World_update_position_x
BEFORE UPDATE OF position_x
ON World
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'position_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_x;
END;

CREATE TRIGGER World_update_position_y
BEFORE UPDATE OF position_y
ON World
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'position_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.position_y;
END;


/*******************
 * Code
 */
CREATE TABLE Code(
  id INTEGER NOT NULL,

  data BLOB NOT NULL,

  PRIMARY KEY(id),
  CHECK(length(data) > 0 AND (length(data) % 4 = 0))
);


/*******************
 * Spawn
 */
CREATE TABLE Spawn(
  id INTEGER NOT NULL,

  code_id INTEGER NOT NULL,

  frequency INTEGER NOT NULL,

  max INTEGER NOT NULL,
  start_x INTEGER NOT NULL,
  start_y INTEGER NOT NULL,
  end_x INTEGER NOT NULL,
  end_y INTEGER NOT NULL,

  energy INTENER NOT NULL,

  PRIMARY KEY(id),
  FOREIGN KEY(code_id) REFERENCES Code(id) ON UPDATE CASCADE ON DELETE CASCADE,
  CHECK(frequency > 0),
  CHECK(start_x >= 0),
  CHECK(start_y >= 0),
  CHECK(end_x > start_x),
  CHECK(end_y > start_y),
  CHECK(max <= (end_x - start_x) * (end_y - start_y)),
  CHECK(energy > 0)
);

/* regions must be inside the world */
CREATE TRIGGER Spawn_insert
BEFORE INSERT
ON Spawn
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_x;
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_y;
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_x;
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_y;
END;

CREATE TRIGGER Spawn_update_start_x
BEFORE UPDATE OF start_x
ON Spawn
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_x;
END;

CREATE TRIGGER Spawn_update_start_y
BEFORE UPDATE OF start_y
ON Spawn
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_y;
END;

CREATE TRIGGER Spawn_update_end_x
BEFORE UPDATE OF end_x
ON Spawn
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_x;
END;

CREATE TRIGGER Spawn_update_end_y
BEFORE UPDATE OF end_y
ON Spawn
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_y;
END;


/*******************
 * Resource
 */
CREATE TABLE Resource(
  id INTEGER NOT NULL,

  frequency INTEGER NOT NULL,

  max INTEGER NOT NULL,
  start_x INTEGER NOT NULL,
  start_y INTEGER NOT NULL,
  end_x INTEGER NOT NULL,
  end_y INTEGER NOT NULL,

  size INTENER NOT NULL,

  PRIMARY KEY(id)
  CHECK(frequency > 0),
  CHECK(start_x >= 0),
  CHECK(start_y >= 0),
  CHECK(end_x > start_x),
  CHECK(end_y > start_y),
  CHECK(max <= (end_x - start_x) * (end_y - start_y)),
  CHECK(size > 0)
);

/* regions must be inside the world */
CREATE TRIGGER Resource_insert
BEFORE INSERT
ON Resource
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_x;
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_y;
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_x;
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_y;
END;

CREATE TRIGGER Resource_update_start_x
BEFORE UPDATE OF start_x
ON Resource
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'start_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_x;
END;

CREATE TRIGGER Resource_update_start_y
BEFORE UPDATE OF start_y
ON Resource
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'start_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) <= NEW.start_y;
END;

CREATE TRIGGER Resource_update_end_x
BEFORE UPDATE OF end_x
ON Resource
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'end_x is out of the World')
  WHERE (SELECT size_x
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_x;
END;

CREATE TRIGGER Resource_update_end_y
BEFORE UPDATE OF end_y
ON Resource
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'end_y is out of the World')
  WHERE (SELECT size_y
         FROM Environment
         WHERE time=(SELECT max(time)
                     FROM Environment)) < NEW.end_y;
END;


/*******************
 * Bug
 */
CREATE TABLE Bug
(
  id INTEGER NOT NULL,

  code_id INTEGER NOT NULL,

  creation INTEGER NOT NULL,
  father_id INTEGER,                    -- NULL if the bug is added manually

  PRIMARY KEY(id),
  FOREIGN KEY(code_id) REFERENCES Code(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(father_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE SET NULL,
  CHECK(creation >= 0)
);

CREATE INDEX Bug_index ON Bug(father_id);

/* creation must be the current time */
CREATE TRIGGER Bug_insert_creation
BEFORE INSERT
ON Bug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'creation is not now')
  WHERE (SELECT max(time)
         FROM Environment) <> NEW.creation;
END;


/*******************
 * Egg
 */
CREATE TABLE Egg
(
  bug_id INTEGER NOT NULL,

  world_id INTEGER NOT NULL,
  energy INTEGER NOT NULL,

  memory_id INTEGER NOT NULL,

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(memory_id) REFERENCES Code(id) ON UPDATE CASCADE ON DELETE CASCADE,
  UNIQUE(world_id),
  UNIQUE(memory_id),
  CHECK(energy > 0)
);


/*******************
 * Registers
 */
CREATE TABLE Registers(
  id INTEGER NOT NULL,

  data BLOB NOT NULL,

  PRIMARY KEY(id),
  CHECK(length(data) = 520)
);


/*******************
 * Alive bug
 */
CREATE TABLE AliveBug
(
  bug_id INTEGER NOT NULL,

  world_id INTEGER NOT NULL,
  birth INTEGER NOT NULL,
  energy INTEGER NOT NULL,

  time_last_action INTEGER,             -- NULL if no action was done yet
  action_time INTEGER,                  -- NULL if the bug is not doing a
                                        -- action

  registers_id INTEGER NOT NULL,
  memory_id INTEGER NOT NULL,

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(registers_id) REFERENCES Registers(id) ON UPDATE CASCADE ON DELETE CASCADE,
  FOREIGN KEY(memory_id) REFERENCES Code(id) ON UPDATE CASCADE ON DELETE CASCADE,
  UNIQUE(world_id),
  UNIQUE(registers_id),
  UNIQUE(memory_id),
  CHECK(time_last_action IS NULL OR time_last_action >= 0),
  CHECK(action_time IS NULL OR action_time > 0),
  CHECK(birth >= 0),
  CHECK(energy > 0)
);

/* Check that the time_last_action isn't in the future */
CREATE TRIGGER AliveBug_insert_time_last_action
BEFORE INSERT
ON AliveBug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The time_last_action is in the future')
  WHERE (SELECT max(time)
         FROM Environment)
        < NEW.time_last_action;
END;

CREATE TRIGGER AliveBug_update_time_last_action
BEFORE UPDATE OF time_last_action
ON AliveBug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The time_last_action is in the future')
  WHERE (SELECT max(time)
         FROM Environment)
        < NEW.time_last_action;
END;

/* Check that the action_time isn't in the past */
CREATE TRIGGER AliveBug_insert_action_time
BEFORE INSERT
ON AliveBug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')
  WHERE (SELECT max(time)
         FROM Environment)
        > NEW.action_time;
END;

CREATE TRIGGER AliveBug_update_action_time
BEFORE UPDATE OF action_time
ON AliveBug
FOR EACH ROW
BEGIN
  SELECT RAISE(ROLLBACK, 'The action_time is in the past')
  WHERE (SELECT max(time)
         FROM Environment)
        > NEW.action_time;
END;

/* Delete the registers when a alive bug is deleted */
CREATE TRIGGER AliveBug_delete
AFTER DELETE
ON AliveBug
FOR EACH ROW
BEGIN
  DELETE FROM Registers
  WHERE id = OLD.registers_id;
END;



/*******************
 * Dead bug
 */
CREATE TABLE DeadBug
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
);

CREATE INDEX DeadBug_index ON DeadBug(killer_id);


/*******************
 * Mutation
 */
CREATE TABLE Mutation
(
  id INTEGER NOT NULL,

  bug_id INTEGER NOT NULL,

  time INTEGER NOT NULL,

  /* type is 0 for mutation, 1 for partial mutation, 2 for permutation,
     3 for addition, 4 for duplication and 5 for deletion */
  type INTEGER NOT NULL,
  position INTEGER NOT NULL,
  original INTEGER,                     -- NULL if new code was added
  mutated INTEGER,                      -- NULL if code was deleted

  PRIMARY KEY(id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id) ON UPDATE CASCADE ON DELETE CASCADE,
  CHECK(time >= 0),
  CHECK(position >= 0),
  CHECK(type >= 0 AND type <= 5),
  CHECK((type == 0 AND original IS NOT NULL AND mutated IS NOT NULL) OR
        (type == 1 AND original IS NOT NULL AND mutated IS NOT NULL) OR
        (type == 2 AND original IS NOT NULL AND mutated IS NOT NULL) OR
        (type == 3 AND original IS NULL AND mutated IS NOT NULL) OR
        (type == 4 AND original IS NULL AND mutated IS NOT NULL) OR
        (type == 5 AND original IS NOT NULL AND mutated IS NULL))
);

CREATE INDEX Mutation_index ON Mutation(bug_id);


/*******************
 * Food
 */
CREATE TABLE Food
(
  id INTEGER NOT NULL,

  time INTEGER NOT NULL,
  world_id INTEGER NOT NULL,
  size INTEGER NOT NULL,

  PRIMARY KEY(id),
  FOREIGN KEY(world_id) REFERENCES World(id) ON UPDATE CASCADE ON DELETE CASCADE,
  UNIQUE(world_id),
  CHECK(time >= 0),
  CHECK(size >= 0)
);


/*******************
 * Stats
 */
CREATE TABLE Stats
(
  id INTEGER NOT NULL,

  time INTEGER NOT NULL,

  families INTEGER NO NULL,
  alive INTEGER NOT NULL,
  eggs INTEGER NOT NULL,
  food INTEGER NOT NULL,
  energy INTEGER NOT NULL,
  mutations INTEGER NOT NULL,
  age INTEGER NOT NULL,

  last_births INTEGER NOT NULL,
  last_sons INTEGER NOT NULL,
  last_deaths INTEGER NOT NULL,
  last_kills INTEGER NOT NULL,
  last_mutations INTEGER NOT NULL,

  PRIMARY KEY(id),
  CHECK(time >= 0),
  CHECK(families >= 0),
  CHECK(alive >= 0),
  CHECK(eggs >= 0),
  CHECK(food >= 0),
  CHECK(energy >= 0),
  CHECK(mutations >= 0),
  CHECK(age >= 0),
  CHECK(last_births >= 0),
  CHECK(last_sons >= 0),
  CHECK(last_deaths >= 0),
  CHECK(last_kills >= 0),
  CHECK(last_mutations >= 0)
);
