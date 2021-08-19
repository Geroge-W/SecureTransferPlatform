USE platform_data;
SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for keysn
-- ----------------------------
DROP TABLE IF EXISTS `keysn`;
CREATE TABLE `keysn` (
  `ikeysn` INT(12) NOT NULL,
  PRIMARY KEY (`ikeysn`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of keysn
-- ----------------------------
INSERT INTO `keysn` VALUES ('1');

-- ----------------------------
-- Table structure for seckeyinfo
-- ----------------------------
DROP TABLE IF EXISTS `seckeyinfo`;
CREATE TABLE `seckeyinfo` (
  `clientid` INT(4) DEFAULT NULL,
  `serverid` INT(4) DEFAULT NULL,
  `keyid` INT(9) NOT NULL,
  `createtime` DATE DEFAULT NULL,
  `state` INT(4) DEFAULT NULL,
  `seckey` VARCHAR(512) DEFAULT NULL,
  PRIMARY KEY (`keyid`),
  KEY `seckeynode_serverid_fk` (`serverid`),
  KEY `IX_seckeyinfo_clientid` (`clientid`),
  CONSTRAINT `seckeynode_clientid_fk` FOREIGN KEY (`clientid`) REFERENCES `secnode` (`id`),
  CONSTRAINT `seckeynode_serverid_fk` FOREIGN KEY (`serverid`) REFERENCES `secnode` (`id`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of seckeyinfo
-- ----------------------------

-- ----------------------------
-- Table structure for secnode
-- ----------------------------
DROP TABLE IF EXISTS `secnode`;
CREATE TABLE `secnode` (
  `id` INT(4) NOT NULL,
  `name` VARCHAR(512) NOT NULL,
  `nodedesc` VARCHAR(512) DEFAULT NULL,
  `createtime` DATE DEFAULT NULL,
  `authcode` INT(12) DEFAULT NULL,
  `state` INT(4) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of secnode
-- ----------------------------
INSERT INTO `secnode` VALUES ('1001', '华东空管局', '上海市松江区涞亭南路925号', '2021-03-26', '4823', '0');
INSERT INTO `secnode` VALUES ('1002', '中南空管局', '广州市白云区南云东街3号', '2021-04-11', '9015', '0');

-- ----------------------------
-- Table structure for srvcfg
-- ----------------------------
DROP TABLE IF EXISTS `srvcfg`; # 服务器信息
CREATE TABLE `srvcfg` (
  `key` VARCHAR(64) DEFAULT NULL,
  `value` VARCHAR(128) DEFAULT NULL
) ENGINE=INNODB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of srvcfg
-- ----------------------------
INSERT INTO `srvcfg` VALUES ('secmng_server_ip', '192.168.78.128');

-- ----------------------------
-- Table structure for tran
-- ----------------------------
DROP TABLE IF EXISTS `tran`;
CREATE TABLE `tran` (
  `iID` INT(12) NOT NULL,
  `tran_operator` INT(12) DEFAULT NULL,
  `trantime` DATE DEFAULT NULL,
  `tranid` INT(4) DEFAULT NULL,
  `trandesc` VARCHAR(512) DEFAULT NULL,
  PRIMARY KEY (`iID`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tran
-- ----------------------------
SET FOREIGN_KEY_CHECKS=1;
