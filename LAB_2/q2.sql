-- Inserting values into the Student table
INSERT INTO Student (Std_rollno, Std_name, Dept, Course1, Course2, Course3, Course4, dob, email) VALUES
(1, 'yog', 'CSE', 'DBMS', 'OS', 'CC', 'CN', '2005-01-01', 'rana@nitt.edu'),
(2, 'cheg', 'CSE', 'Networks', 'CC', 'CN', '2009-02-02', 'amrut@nitt.edu'),
(3, 'santh', 'CSE', 'AIML', 'CC', 'CN', 'CO', '2001-03-03', 'raj@nitt.edu'),
(4, 'aanand', 'CSE', 'CO', 'CC', 'DBMS', 'OS', '2007-07-27', 'charan@nitt.edu'),
(5, 'vis', 'CSE', 'DBMS', 'OS', 'CC', 'CN', '1998-05-05', 'neel@nitt.edu');

-- Dropping columns Course2 and Course3 from the Student table
ALTER TABLE Student DROP COLUMN Course2;
ALTER TABLE Student DROP COLUMN Course3;

-- Modifying the Course1 column's data type to VARCHAR(9)
ALTER TABLE Student ALTER COLUMN Course1 VARCHAR(9);

-- Changing the column name from Std_rollno to Std_rno and setting its data type to INT
ALTER TABLE Student ALTER COLUMN Std_rollno Std_rno ;

-- Updating Course1 from 'DBMS' to 'OS' for relevant rows
UPDATE Student SET Course1 = 'OS' WHERE Course1 = 'DBMS';

-- Deleting rows where Std_name starts with the letter 'S'
DELETE FROM Student WHERE Std_name LIKE 'S%';

-- Selecting all rows where the year of dob is greater than 2005
SELECT * FROM Student WHERE YEAR(dob) > 2005;

-- Dropping the Student table
DROP TABLE Student;

-- Truncating the Student table
TRUNCATE TABLE Student;
