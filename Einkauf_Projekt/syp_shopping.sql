-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Apr 12, 2022 at 03:02 PM
-- Server version: 10.4.22-MariaDB
-- PHP Version: 8.1.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `syp_shopping`
--

-- --------------------------------------------------------

--
-- Table structure for table `accounts`
--

CREATE TABLE `accounts` (
  `Email` varchar(64) NOT NULL,
  `Password` varchar(64) DEFAULT NULL,
  `customersId` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `accounts`
--

INSERT INTO `accounts` (`Email`, `Password`, `customersId`) VALUES
('alice@wonderland.com', 'madhatter', 1);

-- --------------------------------------------------------

--
-- Table structure for table `billingaddr`
--

CREATE TABLE `billingaddr` (
  `id` int(11) NOT NULL,
  `customersId` int(11) DEFAULT NULL,
  `Address` varchar(64) DEFAULT NULL,
  `City` varchar(32) DEFAULT NULL,
  `Zipcode` varchar(16) DEFAULT NULL,
  `Country` varchar(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `billingaddr`
--

INSERT INTO `billingaddr` (`id`, `customersId`, `Address`, `City`, `Zipcode`, `Country`) VALUES
(1, 1, 'Lalaland 123', 'Graz', '8010', 'Austria');

-- --------------------------------------------------------

--
-- Table structure for table `cart`
--

CREATE TABLE `cart` (
  `id` int(11) NOT NULL,
  `totalPrice` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `cartitems`
--

CREATE TABLE `cartitems` (
  `id` int(11) NOT NULL,
  `ProductId` int(11) DEFAULT NULL,
  `Quantity` int(11) DEFAULT NULL,
  `Price` double DEFAULT NULL,
  `totalPrice` double DEFAULT NULL,
  `cartId` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `cartitems`
--

INSERT INTO `cartitems` (`id`, `ProductId`, `Quantity`, `Price`, `totalPrice`, `cartId`) VALUES
(1, 1, 3, 2.5, 7.5, NULL),
(3, 6, 1, 4.9, 4.9, NULL),
(5, 4, 1, 19.95, 19.95, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `customers`
--

CREATE TABLE `customers` (
  `id` int(11) NOT NULL,
  `firstName` varchar(32) DEFAULT NULL,
  `lastName` varchar(32) DEFAULT NULL,
  `Birthdate` date DEFAULT NULL,
  `Phone` varchar(32) DEFAULT NULL,
  `Billing_Addr` int(11) DEFAULT NULL,
  `Shipping_Addr` int(11) DEFAULT NULL,
  `Email` varchar(64) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `customers`
--

INSERT INTO `customers` (`id`, `firstName`, `lastName`, `Birthdate`, `Phone`, `Billing_Addr`, `Shipping_Addr`, `Email`) VALUES
(1, 'Alice', 'Wonderland', '2010-04-03', '06641234567', 1, 1, 'alice@wonderland.com');

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE `orders` (
  `id` int(11) NOT NULL,
  `dateOrdered` date DEFAULT NULL,
  `timeOrdered` time DEFAULT NULL,
  `customerId` int(11) DEFAULT NULL,
  `cartId` int(11) DEFAULT NULL,
  `Billing_Addr` int(11) DEFAULT NULL,
  `Shipping_Addr` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `products`
--

CREATE TABLE `products` (
  `id` int(11) NOT NULL,
  `PrdName` varchar(64) NOT NULL,
  `Quantity` int(11) DEFAULT NULL,
  `Unit` char(2) DEFAULT NULL,
  `Price` double NOT NULL,
  `PrdDesc` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `products`
--

INSERT INTO `products` (`id`, `PrdName`, `Quantity`, `Unit`, `Price`, `PrdDesc`) VALUES
(1, 'Handcreme Monoi', 30, 'ml', 2.5, 'Spendet Feuchtigkeit und verleiht den Händen einen zarten, sonnigen Duft.'),
(2, 'Eau de Toilette Flowers in the City', 100, 'ml', 14.95, 'Ein erfrischendes Parfum – perfekt für die Vorfreude auf den Sommer!'),
(3, 'Eau de Parfum Voile d\'Ocre', 30, 'ml', 19.95, 'Eine neue Sinnlichkeit. Frisch, Strahlend, Prickelnd.'),
(4, 'Eau de Parfum Cuir de NUIT', 30, 'ml', 19.95, 'Ursprung einer Sehnsucht'),
(5, 'Mildes Dusch-Shampoo', 400, 'ml', 3.99, 'Sanftes Duscherlebnis mit einem herrlichen Sommerduft'),
(6, 'Khol Stift', 2, 'g', 4.9, 'Perfekte Betonung');

-- --------------------------------------------------------

--
-- Table structure for table `shippingaddr`
--

CREATE TABLE `shippingaddr` (
  `id` int(11) NOT NULL,
  `customersId` int(11) DEFAULT NULL,
  `Address` varchar(64) DEFAULT NULL,
  `City` varchar(32) DEFAULT NULL,
  `Zipcode` varchar(16) DEFAULT NULL,
  `Country` varchar(32) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `shippingaddr`
--

INSERT INTO `shippingaddr` (`id`, `customersId`, `Address`, `City`, `Zipcode`, `Country`) VALUES
(1, 1, 'Lalaland 123', 'Graz', '8010', 'Austria');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `accounts`
--
ALTER TABLE `accounts`
  ADD PRIMARY KEY (`Email`),
  ADD KEY `customersId` (`customersId`);

--
-- Indexes for table `billingaddr`
--
ALTER TABLE `billingaddr`
  ADD PRIMARY KEY (`id`),
  ADD KEY `customersId` (`customersId`);

--
-- Indexes for table `cart`
--
ALTER TABLE `cart`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `cartitems`
--
ALTER TABLE `cartitems`
  ADD PRIMARY KEY (`id`),
  ADD KEY `ProductId` (`ProductId`),
  ADD KEY `cartId` (`cartId`);

--
-- Indexes for table `customers`
--
ALTER TABLE `customers`
  ADD PRIMARY KEY (`id`),
  ADD KEY `Billing_Addr` (`Billing_Addr`),
  ADD KEY `Email` (`Email`),
  ADD KEY `Shipping_Addr` (`Shipping_Addr`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`id`),
  ADD KEY `customerId` (`customerId`),
  ADD KEY `cartId` (`cartId`),
  ADD KEY `Billing_Addr` (`Billing_Addr`),
  ADD KEY `Shipping_Addr` (`Shipping_Addr`);

--
-- Indexes for table `products`
--
ALTER TABLE `products`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `shippingaddr`
--
ALTER TABLE `shippingaddr`
  ADD PRIMARY KEY (`id`),
  ADD KEY `customersId` (`customersId`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `billingaddr`
--
ALTER TABLE `billingaddr`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `cart`
--
ALTER TABLE `cart`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `cartitems`
--
ALTER TABLE `cartitems`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- AUTO_INCREMENT for table `customers`
--
ALTER TABLE `customers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `orders`
--
ALTER TABLE `orders`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `products`
--
ALTER TABLE `products`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `shippingaddr`
--
ALTER TABLE `shippingaddr`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `accounts`
--
ALTER TABLE `accounts`
  ADD CONSTRAINT `accounts_ibfk_1` FOREIGN KEY (`customersId`) REFERENCES `customers` (`id`);

--
-- Constraints for table `billingaddr`
--
ALTER TABLE `billingaddr`
  ADD CONSTRAINT `billingaddr_ibfk_1` FOREIGN KEY (`customersId`) REFERENCES `customers` (`id`);

--
-- Constraints for table `cartitems`
--
ALTER TABLE `cartitems`
  ADD CONSTRAINT `cartitems_ibfk_1` FOREIGN KEY (`ProductId`) REFERENCES `products` (`id`),
  ADD CONSTRAINT `cartitems_ibfk_2` FOREIGN KEY (`cartId`) REFERENCES `cart` (`id`);

--
-- Constraints for table `customers`
--
ALTER TABLE `customers`
  ADD CONSTRAINT `customers_ibfk_1` FOREIGN KEY (`Billing_Addr`) REFERENCES `billingaddr` (`id`),
  ADD CONSTRAINT `customers_ibfk_2` FOREIGN KEY (`Email`) REFERENCES `accounts` (`Email`),
  ADD CONSTRAINT `customers_ibfk_3` FOREIGN KEY (`Shipping_Addr`) REFERENCES `shippingaddr` (`id`);

--
-- Constraints for table `orders`
--
ALTER TABLE `orders`
  ADD CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`customerId`) REFERENCES `customers` (`id`),
  ADD CONSTRAINT `orders_ibfk_2` FOREIGN KEY (`cartId`) REFERENCES `cart` (`id`),
  ADD CONSTRAINT `orders_ibfk_3` FOREIGN KEY (`Billing_Addr`) REFERENCES `billingaddr` (`id`),
  ADD CONSTRAINT `orders_ibfk_4` FOREIGN KEY (`Shipping_Addr`) REFERENCES `shippingaddr` (`id`);

--
-- Constraints for table `shippingaddr`
--
ALTER TABLE `shippingaddr`
  ADD CONSTRAINT `shippingaddr_ibfk_1` FOREIGN KEY (`customersId`) REFERENCES `customers` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
