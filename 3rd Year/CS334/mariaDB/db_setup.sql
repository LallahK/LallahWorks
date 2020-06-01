

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";



CREATE TABLE IF NOT EXISTS `apply` 
(
  
`sID` int(11) NOT NULL,
  
`cName` varchar(128) NOT NULL,
  
`major` varchar(128) NOT NULL,
  
`decision` varchar(1) NOT NULL,
  

PRIMARY KEY 
(`sID`,`cName`,`major`)


) 

ENGINE=InnoDB DEFAULT CHARSET=latin1;





INSERT INTO `apply` 
(`sID`, `cName`, `major`, `decision`) 

VALUES
(123, 'Berkeley', 'CS', 'Y'),

(123, 'Cornell', 'EE', 'Y'),

(123, 'Stanford', 'CS', 'Y'),

(123, 'Stanford', 'EE', 'N'),

(234, 'Berkeley', 'biology', 'N'),

(345, 'Cornell', 'bioengineering', 'N'),

(345, 'Cornell', 'CS', 'Y'),

(345, 'Cornell', 'EE', 'N'),

(345, 'MIT', 'bioengineering', 'Y'),

(543, 'MIT', 'CS', 'N'),

(678, 'Stanford', 'history', 'Y'),

(765, 'Cornell', 'history', 'N'),

(765, 'Cornell', 'psychology', 'Y'),

(765, 'Stanford', 'history', 'Y'),

(876, 'MIT', 'biology', 'Y'),

(876, 'MIT', 'marine biology', 'N'),

(876, 'Stanford', 'CS', 'N'),

(987, 'Berkeley', 'CS', 'Y'),

(987, 'Stanford', 'CS', 'Y');




CREATE TABLE IF NOT EXISTS `college` 

(
  
`cName` varchar(128) NOT NULL,

`state` varchar(2) NOT NULL,
  
`enrollment` int(11) NOT NULL,
  
PRIMARY KEY (`cName`)

) 

ENGINE=InnoDB DEFAULT CHARSET=latin1;




INSERT INTO `college` 
(`cName`, `state`, `enrollment`) 
VALUES

('Berkeley', 'CA', 36000),

('Cornell', 'NY', 21000),

('MIT', 'MA', 10000),

('Stanford', 'CA', 15000)
;




CREATE TABLE IF NOT EXISTS `student` 
(
  
`sID` int(11) NOT NULL,

`sName` varchar(30) NOT NULL,
`GPA` float NOT NULL,
  
`sizeHS` int(11) NOT NULL,
  
PRIMARY KEY (`sID`)

) 

ENGINE=InnoDB DEFAULT CHARSET=latin1;





INSERT INTO `student` 
(`sID`, `sName`, `GPA`, `sizeHS`) 
VALUES

(123, 'Amy', 3.9, 1000),

(234, 'Bob', 3.5, 1500),

(345, 'Craig', 3.5, 500),

(456, 'Doris', 3.9, 1000),

(543, 'Craig', 3.4, 2000),

(567, 'Edward', 2.9, 2000),

(654, 'Amy', 3.9, 1000),

(678, 'Fay', 3.8, 200),

(765, 'Jay', 2.9, 1500),

(789, 'Gary', 3.4, 800),

(876, 'Irene', 3.9, 400),

(987, 'Helen', 3.7, 800);




