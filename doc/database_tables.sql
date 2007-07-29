/*******************
 * Environment
 */
CREATE TABLE Environment
(
  id INTEGER NOT NULL,

  time INTEGER NOT NULL,

  next_bug INTEGER,                     -- NULL if there aren't alive bugs

  -- the rest of the rows can't change
  size_x INTEGER NOT NULL,
  size_y INTEGER NOT NULL,

  energy_developed INTEGER NOT NULL,

  mutations_percent REAL NOT NULL,

  time_birth INTEGER NOT NULL,

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
  CHECK(energy_developed >= 0),
  CHECK(mutations_percent >= 0),
  CHECK(time_birth >= 0),
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
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'There is a older time')
  WHERE (SELECT max(time)
         FROM Environment) > NEW.time;
END;

CREATE TRIGGER Environment_update_time
BEFORE UPDATE
ON Environment
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'There is a older time')
  WHERE (SELECT max(time)
         FROM Environment) >= NEW.time;
END;

/* Check if next_bug is a valid id */
CREATE TRIGGER Environment_insert_next_bug
BEFORE INSERT
ON Environment
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'ID of the Next bug not found')
  WHERE (SELECT id
         FROM bug
         WHERE id = NEW.next_bug) IS NULL;
END;

CREATE TRIGGER Environment_update_next_bug
BEFORE UPDATE
ON Environment
FOR EACH ROW BEGIN
SELECT RAISE(ROLLBACK, 'ID of the Next bug not found')
  WHERE (SELECT id
         FROM bug
         WHERE id = NEW.next_bug) IS NULL;
END;

/* Only id, time and next_bug can change */
CREATE TRIGGER Environment_update_valid_time
BEFORE UPDATE
ON Environment
FOR EACH ROW
WHEN OLD.size_x != NEW.size_x OR
     OLD.size_y != NEW.size_y OR
     OLD.energy_developed != NEW.energy_developed OR
     OLD.mutations_percent != NEW.mutations_percent OR
     OLD.time_birth != NEW.time_birth OR
     OLD.energy_nothing != NEW.energy_nothing OR
     OLD.energy_myself != NEW.energy_myself OR
     OLD.energy_detect != NEW.energy_detect OR
     OLD.energy_info != NEW.energy_info OR
     OLD.energy_move != NEW.energy_move OR
     OLD.energy_turn != NEW.energy_turn OR
     OLD.energy_attack != NEW.energy_attack OR
     OLD.energy_eat != NEW.energy_eat OR
     OLD.energy_egg != NEW.energy_egg
BEGIN
  SELECT RAISE(ROLLBACK, 'changed invalid col');
END;



/*******************
 * Bug
 */
CREATE TABLE Bug
(
  id INTEGER NOT NULL,

  energy INTEGER NOT NULL,

  position_x INTEGER NOT NULL,
  position_y INTEGER NOT NULL,
  orientation INTEGER NOT NULL,

  birth INTEGER NOT NULL,
  dead INTEGER,                         -- NULL if alive

  father_id INTEGER,                    -- NULL if developed bug
  killer_id INTEGER,                    -- NULL if alive

  PRIMARY KEY(id),
  FOREIGN KEY(killer_id) REFERENCES Bug(id),
  FOREIGN KEY(father_id) REFERENCES Bug(id),
  CHECK(position_x >= 0 AND position_y >= 0),
  CHECK(orientation >= 0 AND orientation <= 3), -- 4 possible orientations
  CHECK(birth >= 0),
  CHECK(dead IS NULL OR (birth <= dead)), -- If dead IS NOT NULL
  CHECK(killer_id IS NULL OR (killer_id IS NOT NULL AND dead IS NOT NULL))
);

CREATE INDEX Bug_index ON Bug(id);
CREATE INDEX Bug_position_index ON Bug(position_x, position_y);
CREATE INDEX Bug_dead_index ON Bug(dead);

CREATE VIEW Bug_alive AS SELECT * FROM Bug WHERE dead IS NULL;

/* Check if the id will change */
CREATE TRIGGER Bug_id_trigger
BEFORE UPDATE
ON Bug
FOR EACH ROW
WHEN OLD.id != NEW.id
BEGIN
  SELECT RAISE(ROLLBACK, 'id changed');
END;

/* Check if the bug is alive before update */
CREATE TRIGGER Bug_alive_trigger
BEFORE UPDATE
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'Bug dead')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.id AND dead IS NULL)
        IS NULL;
END;

/* Check if father_id is a valid id */
CREATE TRIGGER Bug_insert_father_trigger
BEFORE INSERT
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'Father not found')
  WHERE NEW.father_id IS NOT NULL
         AND
         (SELECT id
          FROM Bug
          WHERE id=NEW.father_id AND dead IS NULL) IS NULL;
END;

CREATE TRIGGER Bug_update_father_trigger
BEFORE UPDATE
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'Father not found')
  WHERE NEW.father_id IS NOT NULL
         AND
         (SELECT id
          FROM Bug
          WHERE id=NEW.father_id AND dead IS NULL) IS NULL;
END;

/* Check if killer_id is a valid id */
CREATE TRIGGER Bug_insert_killer_trigger
BEFORE INSERT
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'Killer not found')
  WHERE NEW.killer_id IS NOT NULL
        AND
        (SELECT id
         FROM Bug
         WHERE id=NEW.killer_id AND dead IS NULL) IS NULL;
END;

CREATE TRIGGER Bug_update_killer_trigger
BEFORE UPDATE
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'Killer not found')
  WHERE NEW.killer_id IS NOT NULL
        AND
        (SELECT id
         FROM Bug
         WHERE id=NEW.killer_id AND dead IS NULL) IS NULL;
END;

/* Check that two bugs are not in the same position */
CREATE TRIGGER Bug_insert_position_trigger
BEFORE INSERT
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'There is a bug in the same position')
  WHERE (SELECT id
         FROM Bug
         WHERE dead IS NULL AND position_x=NEW.position_x AND position_y=NEW.position_y)
        IS NOT NULL;
END;

CREATE TRIGGER Bug_update_position_trigger
BEFORE UPDATE
ON Bug
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'There is a bug in the same position')
  WHERE (SELECT id
         FROM Bug
         WHERE dead IS NULL AND position_x=NEW.position_x AND position_y=NEW.position_y)
        IS NOT NULL;
END;

/* Propagate the dead */
CREATE TRIGGER Bug_update_dead_trigger
BEFORE UPDATE
ON Bug
FOR EACH ROW
WHEN OLD.dead IS NULL AND NEW.dead IS NOT NULL
BEGIN
  DELETE FROM CPU WHERE bug_id = OLD.id;
END;

/* Propagate the deletion */
CREATE TRIGGER Bug_delete_trigger
BEFORE DELETE
ON Bug
FOR EACH ROW BEGIN
  DELETE FROM Code WHERE bug_id = OLD.id;
  DELETE FROM CPU WHERE bug_id = OLD.id;
END;


/*******************
 * Code
 */
CREATE TABLE Code
(
  bug_id INTEGER NOT NULL,

  size INTEGER NOT NULL,
  md5 CHAR[32] NOT NULL,

  /* The blob is the last col for performance */
  code BLOB NOT NULL,

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id),
  CHECK(size >= 0 AND (size % 4 == 0)),
  CHECK(length(md5) == 32),
  CHECK(length(code) % 4 == 0),
  CHECK(size == length(code))
);

CREATE INDEX Code_index ON Code(bug_id);

/* Check if the id will change */
CREATE TRIGGER Code_update_id_trigger
BEFORE UPDATE
ON Code
FOR EACH ROW
WHEN OLD.bug_id != NEW.bug_id
BEGIN
  SELECT RAISE(ROLLBACK, 'id changed');
END;

/* Make sure that the bug_id is valid */
CREATE TRIGGER Code_insert_valid_trigger
BEFORE INSERT
ON Code
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug not found')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;
END;

CREATE TRIGGER Code_update_valid_trigger
BEFORE UPDATE
ON Code
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug not found')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;
END;

/* Make sure that the bug_id is unique */
CREATE TRIGGER Code_insert_uniq_trigger
BEFORE INSERT
ON Code
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug already in table')
  WHERE (SELECT count(bug_id)
         FROM Code
         WHERE bug_id=NEW.bug_id) != 0;
END;

CREATE TRIGGER Code_update_uniq_trigger
BEFORE UPDATE
ON Code
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug already in table')
  WHERE (SELECT count(bug_id)
         FROM Code
         WHERE bug_id=NEW.bug_id) != 0;
END;

/* Propagate the deletion */
CREATE TRIGGER Code_delete_trigger
BEFORE DELETE
ON Bug
FOR EACH ROW BEGIN
  DELETE FROM Mutation WHERE bug_id = OLD.id;
END;


/*******************
 * CPU
 */
CREATE TABLE CPU
(
  bug_id INTEGER NOT NULL,

  /* CPU registers */
  r0 INTEGER NOT NULL DEFAULT 0,
  r1 INTEGER NOT NULL DEFAULT 0,
  r2 INTEGER NOT NULL DEFAULT 0,
  r3 INTEGER NOT NULL DEFAULT 0,
  r4 INTEGER NOT NULL DEFAULT 0,
  r5 INTEGER NOT NULL DEFAULT 0,
  r6 INTEGER NOT NULL DEFAULT 0,
  r7 INTEGER NOT NULL DEFAULT 0,
  r8 INTEGER NOT NULL DEFAULT 0,
  r9 INTEGER NOT NULL DEFAULT 0,
  r10 INTEGER NOT NULL DEFAULT 0,
  r11 INTEGER NOT NULL DEFAULT 0,
  r12 INTEGER NOT NULL DEFAULT 0,
  pc INTEGER NOT NULL DEFAULT 0,
  sp INTEGER NOT NULL DEFAULT 0,
  itp INTEGER NOT NULL DEFAULT 0,

  PRIMARY KEY(bug_id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id)
);

CREATE INDEX CPU_index ON CPU(bug_id);

/* Check if the id will change */
CREATE TRIGGER CPU_update_id_trigger
BEFORE UPDATE
ON CPU
FOR EACH ROW
WHEN OLD.bug_id != NEW.bug_id
BEGIN
  SELECT RAISE(ROLLBACK, 'id changed');
END;

/* Make sure that the bug_id is valid */
CREATE TRIGGER CPU_insert_valid_trigger
BEFORE INSERT
ON CPU
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug not found')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;
END;

CREATE TRIGGER CPU_update_valid_trigger
BEFORE UPDATE
ON CPU
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug not found')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;
END;

/* Make sure that the bug_id is unique */
CREATE TRIGGER CPU_insert_uniq_trigger
BEFORE INSERT
ON CPU
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug already in table')
  WHERE (SELECT count(bug_id)
         FROM CPU
         WHERE bug_id=NEW.bug_id) != 0;
END;

CREATE TRIGGER CPU_update_uniq_trigger
BEFORE UPDATE
ON CPU
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug already in table')
  WHERE (SELECT count(bug_id)
         FROM CPU
         WHERE bug_id=NEW.bug_id) != 0;
END;


/*******************
 * Mutation
 */
CREATE TABLE Mutation
(
  id INTEGER NOT NULL,

  position INTEGER NOT NULL,
  /* original and mutated can't be NULL at same time */
  original INTEGER,                     -- NULL if new code was added
  mutated INTEGER,                      -- NULL if code was deleted

  bug_id INTEGER NOT NULL,

  PRIMARY KEY(id),
  FOREIGN KEY(bug_id) REFERENCES Bug(id),
  CHECK(position >= 0),
  CHECK(original >= 0),                 -- If original IS NOT NULL
  CHECK(mutated >= 0)                   -- If mutated IS NOT NULL 
);

CREATE INDEX Mutation_index ON Mutation(bug_id);

/* Make sure that the bug_id is valid */
CREATE TRIGGER Mutation_insert_trigger
BEFORE INSERT
ON Mutation
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug not found')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;
END;

CREATE TRIGGER Mutation_update_trigger
BEFORE UPDATE
ON Mutation
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'id of bug not found')
  WHERE (SELECT id
         FROM Bug
         WHERE id=NEW.bug_id AND dead IS NULL) IS NULL;
END;



/*******************
 * Food
 */
CREATE TABLE Food
(
  id INTEGER NOT NULL,


  position_x INTEGER NOT NULL,
  position_y INTEGER NOT NULL,

  size INTEGER NOT NULL,

  PRIMARY KEY(id),
  CHECK(position_x >= 0 AND position_y >= 0),
  CHECK(size >= 0)
);

CREATE INDEX Food_index ON Food(id);

/* Check that two food elements are not in the same position */
CREATE TRIGGER Food_insert_position_trigger
BEFORE INSERT
ON Food
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'There is food in the same position')
  WHERE (SELECT id
         FROM Food
         WHERE position_x=NEW.position_x AND position_y=NEW.position_y)
        IS NOT NULL;
END;

CREATE TRIGGER Food_update_position_trigger
BEFORE UPDATE
ON Food
FOR EACH ROW BEGIN
  SELECT RAISE(ROLLBACK, 'There is food in the same position')
  WHERE (SELECT id
         FROM Food
         WHERE position_x=NEW.position_x AND position_y=NEW.position_y)
        IS NOT NULL;
END;
