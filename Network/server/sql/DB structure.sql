CREATE TABLE `Logger` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime DEFAULT CURRENT_TIMESTAMP COMMENT 'Дата создания записи',
  `message` varchar(4048) DEFAULT NULL COMMENT 'USB-трафик',
  PRIMARY KEY (`id`)
)
ENGINE = INNODB
COMMENT = 'Таблица для хранения USB-трафика';



CREATE TABLE `State` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_logger` int(11) DEFAULT NULL COMMENT ' id USB-трафика {Logger}',
  `modify_date` datetime DEFAULT NULL COMMENT 'Дата изменения записи',
  `is_read` tinyint(1) DEFAULT 0 COMMENT 'Флаг непрочитанной записи',
  `deleted` tinyint(1) DEFAULT 0 COMMENT 'Флаг удаленной записи',
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_state_id_logger` FOREIGN KEY (`id_logger`)
  REFERENCES `Logger` (`id`) ON DELETE NO ACTION ON UPDATE RESTRICT
)
ENGINE = INNODB
COMMENT = 'Таблица состояния USB-трафика';


DELIMITER $$

CREATE TRIGGER InsertStateForLogRow AFTER INSERT ON Logger
	FOR EACH ROW
	BEGIN
	  INSERT INTO State (id_logger) VALUES
		(new.id);
	END; $$

DELIMITER ;
