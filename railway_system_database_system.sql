-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 23, 2022 at 05:25 AM
-- Server version: 10.4.24-MariaDB
-- PHP Version: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `railway system database system`
--

-- --------------------------------------------------------

--
-- Table structure for table `administrator`
--

CREATE TABLE `administrator` (
  `Admin_ID` varchar(5) COLLATE utf8mb4_bin NOT NULL,
  `Admin_Name` varchar(50) COLLATE utf8mb4_bin NOT NULL,
  `Admin_IC` char(12) COLLATE utf8mb4_bin NOT NULL,
  `Admin_Password` varchar(8) COLLATE utf8mb4_bin NOT NULL,
  `Admin_In_Charge` char(1) COLLATE utf8mb4_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `administrator`
--

INSERT INTO `administrator` (`Admin_ID`, `Admin_Name`, `Admin_IC`, `Admin_Password`, `Admin_In_Charge`) VALUES
('A1', 'Jason', '000111070111', 'A000', 'y'),
('A2', 'Derulo', '000222070222', 'A001', 'n');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `Customer_IC` char(12) COLLATE utf8mb4_bin NOT NULL,
  `Admin_ID` varchar(5) COLLATE utf8mb4_bin DEFAULT NULL,
  `Customer_Age` int(11) NOT NULL,
  `Customer_Local` char(1) COLLATE utf8mb4_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- --------------------------------------------------------

--
-- Table structure for table `price_update`
--

CREATE TABLE `price_update` (
  `Price_Update_ID` int(11) NOT NULL,
  `Base_Price` double(5,2) NOT NULL,
  `Privilage_Price` double(5,2) NOT NULL,
  `Senior_Discount` double(5,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `price_update`
--

INSERT INTO `price_update` (`Price_Update_ID`, `Base_Price`, `Privilage_Price`, `Senior_Discount`) VALUES
(1, 10.00, 8.00, 10.00);

-- --------------------------------------------------------

--
-- Table structure for table `railway_driver`
--

CREATE TABLE `railway_driver` (
  `Driver_ID` varchar(5) COLLATE utf8mb4_bin NOT NULL,
  `Railway_ID` varchar(5) COLLATE utf8mb4_bin DEFAULT NULL,
  `Driver_Name` varchar(50) COLLATE utf8mb4_bin NOT NULL,
  `Driver_IC` char(12) COLLATE utf8mb4_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `railway_driver`
--

INSERT INTO `railway_driver` (`Driver_ID`, `Railway_ID`, `Driver_Name`, `Driver_IC`) VALUES
('RD1', 'R001', 'Peter', '990317070111'),
('RD2', 'R002', 'Griffin', '800420070114'),
('RD3', 'R003', 'Lois', '970724070112'),
('RD4', 'R004', 'Brian', '870110070457'),
('RD5', 'R005', 'Meg', '960521070864');

-- --------------------------------------------------------

--
-- Table structure for table `railway_transport`
--

CREATE TABLE `railway_transport` (
  `Railway_ID` varchar(5) COLLATE utf8mb4_bin NOT NULL,
  `Code` char(1) COLLATE utf8mb4_bin NOT NULL,
  `Capacity` smallint(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `railway_transport`
--

INSERT INTO `railway_transport` (`Railway_ID`, `Code`, `Capacity`) VALUES
('R001', 'A', 40),
('R002', 'B', 30),
('R003', 'C', 50),
('R004', 'D', 40),
('R005', 'E', 30);

-- --------------------------------------------------------

--
-- Table structure for table `report`
--

CREATE TABLE `report` (
  `Report_ID` varchar(5) COLLATE utf8mb4_bin NOT NULL,
  `Admin_ID` varchar(5) COLLATE utf8mb4_bin DEFAULT NULL,
  `Report_Type` varchar(10) COLLATE utf8mb4_bin DEFAULT NULL,
  `Type_Range` varchar(15) COLLATE utf8mb4_bin DEFAULT NULL,
  `Total_Ticket` int(11) DEFAULT NULL,
  `Total_Sales` double(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- --------------------------------------------------------

--
-- Table structure for table `ticket`
--

CREATE TABLE `ticket` (
  `Customer_IC` char(12) COLLATE utf8mb4_bin DEFAULT NULL,
  `Trip_ID` varchar(5) COLLATE utf8mb4_bin DEFAULT NULL,
  `Ticket_ID` varchar(5) COLLATE utf8mb4_bin NOT NULL,
  `Seat_Number` char(3) COLLATE utf8mb4_bin NOT NULL,
  `Date_Purchase` date NOT NULL,
  `Final_Price` double(7,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- --------------------------------------------------------

--
-- Table structure for table `trip`
--

CREATE TABLE `trip` (
  `Trip_ID` varchar(5) COLLATE utf8mb4_bin NOT NULL,
  `Railway_ID` varchar(5) COLLATE utf8mb4_bin DEFAULT NULL,
  `Departed_Station` varchar(30) COLLATE utf8mb4_bin NOT NULL,
  `Destinated_Station` varchar(30) COLLATE utf8mb4_bin NOT NULL,
  `Trip_Date` date NOT NULL,
  `Time_Depart` time NOT NULL,
  `Time_Arrive` time NOT NULL,
  `Duration` int(11) NOT NULL,
  `Price` double(7,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `trip`
--

INSERT INTO `trip` (`Trip_ID`, `Railway_ID`, `Departed_Station`, `Destinated_Station`, `Trip_Date`, `Time_Depart`, `Time_Arrive`, `Duration`, `Price`) VALUES
('T001', 'R001', 'PEN', 'IPOH', '2022-09-14', '05:15:00', '06:55:00', 100, 100.00),
('T002', 'R003', 'PEN', 'PB', '2022-10-09', '05:20:00', '06:20:00', 60, 60.00),
('T003', 'R002', 'PEN', 'TAIPING', '2022-10-14', '06:20:00', '07:10:00', 50, 50.00),
('T004', 'R001', 'PEN', 'GEMAS', '2022-03-12', '07:50:00', '14:50:00', 420, 420.00);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `administrator`
--
ALTER TABLE `administrator`
  ADD PRIMARY KEY (`Admin_ID`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`Customer_IC`),
  ADD KEY `Admin_ID` (`Admin_ID`);

--
-- Indexes for table `price_update`
--
ALTER TABLE `price_update`
  ADD PRIMARY KEY (`Price_Update_ID`);

--
-- Indexes for table `railway_driver`
--
ALTER TABLE `railway_driver`
  ADD PRIMARY KEY (`Driver_ID`),
  ADD KEY `Railway_ID` (`Railway_ID`);

--
-- Indexes for table `railway_transport`
--
ALTER TABLE `railway_transport`
  ADD PRIMARY KEY (`Railway_ID`);

--
-- Indexes for table `report`
--
ALTER TABLE `report`
  ADD PRIMARY KEY (`Report_ID`),
  ADD KEY `Admin_ID` (`Admin_ID`);

--
-- Indexes for table `ticket`
--
ALTER TABLE `ticket`
  ADD PRIMARY KEY (`Ticket_ID`),
  ADD KEY `Customer_IC` (`Customer_IC`),
  ADD KEY `Trip_ID` (`Trip_ID`);

--
-- Indexes for table `trip`
--
ALTER TABLE `trip`
  ADD PRIMARY KEY (`Trip_ID`),
  ADD KEY `Railway_ID` (`Railway_ID`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `customer`
--
ALTER TABLE `customer`
  ADD CONSTRAINT `customer_ibfk_1` FOREIGN KEY (`Admin_ID`) REFERENCES `administrator` (`Admin_ID`);

--
-- Constraints for table `railway_driver`
--
ALTER TABLE `railway_driver`
  ADD CONSTRAINT `railway_driver_ibfk_1` FOREIGN KEY (`Railway_ID`) REFERENCES `railway_transport` (`Railway_ID`) ON DELETE CASCADE;

--
-- Constraints for table `report`
--
ALTER TABLE `report`
  ADD CONSTRAINT `report_ibfk_1` FOREIGN KEY (`Admin_ID`) REFERENCES `administrator` (`Admin_ID`);

--
-- Constraints for table `ticket`
--
ALTER TABLE `ticket`
  ADD CONSTRAINT `ticket_ibfk_1` FOREIGN KEY (`Customer_IC`) REFERENCES `customer` (`Customer_IC`),
  ADD CONSTRAINT `ticket_ibfk_2` FOREIGN KEY (`Trip_ID`) REFERENCES `trip` (`Trip_ID`) ON DELETE CASCADE;

--
-- Constraints for table `trip`
--
ALTER TABLE `trip`
  ADD CONSTRAINT `trip_ibfk_1` FOREIGN KEY (`Railway_ID`) REFERENCES `railway_transport` (`Railway_ID`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
