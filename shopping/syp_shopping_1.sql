-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Apr 27, 2022 at 04:28 PM
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
-- Database: `syp_shopping_1`
--

-- --------------------------------------------------------

--
-- Table structure for table `cartitems`
--

CREATE TABLE `cartitems` (
  `id` int(11) NOT NULL,
  `ProductId` int(11) DEFAULT NULL,
  `Quantity` int(11) DEFAULT NULL,
  `totalPrice` double DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `category`
--

CREATE TABLE `category` (
  `Id` int(10) NOT NULL,
  `categoryName` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `category`
--

INSERT INTO `category` (`Id`, `categoryName`) VALUES
(1, 'GESICHT'),
(2, 'MAKE-UP'),
(3, 'PARFUM'),
(4, 'KÖRPER'),
(5, 'HAARE');

-- --------------------------------------------------------

--
-- Table structure for table `customers`
--

CREATE TABLE `customers` (
  `id` int(11) NOT NULL,
  `firstName` varchar(32) NOT NULL,
  `lastName` varchar(32) NOT NULL,
  `gender` varchar(16) NOT NULL,
  `birthDate` date NOT NULL,
  `street` varchar(64) NOT NULL,
  `housNr` int(10) NOT NULL,
  `ort` varchar(32) NOT NULL,
  `plz` int(10) NOT NULL,
  `phone` int(32) DEFAULT NULL,
  `email` varchar(64) NOT NULL,
  `password` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `customers`
--

INSERT INTO `customers` (`id`, `firstName`, `lastName`, `gender`, `birthDate`, `street`, `housNr`, `ort`, `plz`, `phone`, `email`, `password`) VALUES
(1, 'Ly', 'Tran', '', '1111-12-10', '', 0, '', 0, 2147483647, '', ''),
(2, 'Julia Neu', 'Alexander', 'Herr', '2022-04-04', 'hackergasse', 158, 'Graz', 8010, 1650008107, 'julia@gmail', '202cb962ac59075b964b07152d234b70'),
(3, 'Johannes', 'Wolf', 'Herr', '2022-03-29', 'Körösistraße', 123, 'Graz', 8562, 1650008107, 'johannes@gmail.com', '202cb962ac59075b964b07152d234b70'),
(4, 'Mi', 'Do', 'Herr', '2222-03-18', 'Fischergasse', 33, 'Graz', 8010, 2147483647, 'mido@gmail.com', '94732b4641dcdd41d43f779535d17d0e'),
(6, 'Maskros', 'Do', 'Herr', '2022-03-28', 'Fischergasse', 33, 'Graz', 8010, 2147483647, 'mi@gmail.com', 'afb0b97df87090596ae7c503f60bb23f'),
(7, 'andreas', 'seidl', 'Herr', '1998-10-09', 'Sigmundstadl', 13, 'Graz', 8020, 2147483647, 'seidl.andreas@mailbox.org', '202cb962ac59075b964b07152d234b70');

-- --------------------------------------------------------

--
-- Table structure for table `ordereditems`
--

CREATE TABLE `ordereditems` (
  `orderID` int(11) DEFAULT NULL,
  `ProductId` int(11) DEFAULT NULL,
  `Quantity` int(11) DEFAULT NULL,
  `purchased_amount` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `ordereditems`
--

INSERT INTO `ordereditems` (`orderID`, `ProductId`, `Quantity`, `purchased_amount`) VALUES
(1, 14, 3, 126),
(1, 3, 4, 79.8),
(1, 1, 2, 5),
(1, 4, 3, 59.85),
(1, 39, 1, 39.9),
(2, 3, 1, 19.95),
(2, 4, 2, 39.9),
(2, 27, 1, 59),
(2, 5, 1, 3.99),
(19, 19, 3, 17.97),
(20, 21, 1, 4.99),
(20, 3, 1, 19.95),
(20, 2, 2, 29.9);

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE `orders` (
  `id` int(11) NOT NULL,
  `totalPrice` float NOT NULL,
  `dateOrdered` date DEFAULT NULL,
  `timeOrdered` time DEFAULT NULL,
  `customerId` int(11) DEFAULT NULL,
  `shippingCostId` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `orders`
--

INSERT INTO `orders` (`id`, `totalPrice`, `dateOrdered`, `timeOrdered`, `customerId`, `shippingCostId`) VALUES
(1, 310.55, '2022-04-23', '13:27:36', 4, 2),
(2, 122.84, '2022-04-27', '15:10:55', 6, 2),
(3, 0, '2022-04-27', '15:24:37', 6, 1),
(4, 0, '2022-04-27', '15:31:47', 6, 1),
(5, 0, '2022-04-27', '15:32:52', 6, 1),
(6, 0, '2022-04-27', '15:34:35', 6, 1),
(7, 0, '2022-04-27', '15:39:08', 6, 1),
(8, 0, '2022-04-27', '15:44:43', 6, 1),
(9, 0, '2022-04-27', '15:46:13', 6, 1),
(10, 0, '2022-04-27', '15:46:28', 6, 1),
(11, 0, '2022-04-27', '15:46:43', 6, 1),
(12, 0, '2022-04-27', '15:47:19', 6, 1),
(13, 0, '2022-04-27', '15:47:31', 6, 1),
(14, 0, '2022-04-27', '15:48:04', 6, 1),
(15, 0, '2022-04-27', '15:49:25', 6, 1),
(16, 0, '2022-04-27', '15:49:55', 6, 1),
(17, 0, '2022-04-27', '15:52:04', 6, 1),
(18, 0, '2022-04-27', '15:53:19', 6, 1),
(19, 17.97, '2022-04-27', '16:12:54', 6, 1),
(20, 54.84, '2022-04-27', '16:19:12', 7, 2);

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
  `PrdDesc` varchar(255) DEFAULT NULL,
  `Img` varchar(100) DEFAULT NULL,
  `catId` int(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `products`
--

INSERT INTO `products` (`id`, `PrdName`, `Quantity`, `Unit`, `Price`, `PrdDesc`, `Img`, `catId`) VALUES
(1, 'Handcreme Monoi', 30, 'ml', 2.5, 'Spendet Feuchtigkeit und verleiht den Händen einen zarten, sonnigen Duft.', 'monoi-handcreme.jpg', 4),
(2, 'Eau de Toilette Flowers in the City', 100, 'ml', 14.95, 'Ein erfrischendes Parfum – perfekt für die Vorfreude auf den Sommer!', 'flowersincity.jpg', 3),
(3, 'Eau de Parfum Voile d\'Ocre', 30, 'ml', 19.95, 'Eine neue Sinnlichkeit. Frisch, Strahlend, Prickelnd.', 'voiledocre.jpg', 3),
(4, 'Eau de Parfum Cuir de NUIT', 30, 'ml', 19.95, 'Ursprung einer Sehnsucht', 'cuirdenuit.jpg', 3),
(5, 'Mildes Dusch-Shampoo', 400, 'ml', 3.99, 'Sanftes Duscherlebnis mit einem herrlichen Sommerduft', 'mildes-dusch-shampoo.jpg', 4),
(6, 'Khol Stift', 2, 'g', 4.9, 'Perfekte Betonung', 'kohl-stift.jpg', 2),
(7, 'Haarbalsam', 150, 'ml', 9.99, '1 Haarbalsam-1000 Möglichkeiten- Intensive Reparatur', 'haarbalsam.jpg', 5),
(8, 'Festes Shampoo Glanz', 60, 'g', 6.99, 'Weniger Plastik, Mehr Beauty', 'festes-shamp-glanz.jpg', 5),
(9, 'Detox Peeling - Kopfhaut', 35, 'g', 2.99, 'Peelt und reinigt die Kopfhaut', 'detox-kopfhaut.jpg', 5),
(10, 'Purifying Maske - Fettiger Ansatz', 35, 'ml', 2.99, 'Absorbiert Talgüberschuss und reinigt den Haaransatz nachhaltig', 'purifying-maske.jpg', 5),
(11, 'Aufpolsternde Tagespflege', 50, 'ml', 35.9, 'Die Haut wird sofort mit Feuchtigkeit versorgt, geglättet und aufgepolstert.', 'aufpolsternde-tagespflege.jpg', 4),
(12, 'Glow Emulsion SPF 30', 50, 'ml', 59, 'Korrektur und Vorbeugung vor den Zeichen der Zeit', 'glow-emulsion.jpg', 2),
(13, 'Anti-Pigmentflecken Konzentrat', 14, 'ml', 35.9, 'orrigiert die Zeichen der Zeit und verschönert die Haut', 'antipigment-konzentrat.jpg', 2),
(14, 'Anti-Falten Intensivpflege Gesicht, Hals', 75, 'ml', 42, 'Die Haut wird sofort geglättet und aufgepolstert.', 'antifalten-gesicht-hals.jpg', 1),
(15, '2 in 1 Mizellenwasser', 200, 'ml', 9.9, 'Entfernt Make-up, reinigt das Gesicht und die Augen und versorgt sie mit Feuchtigkeit', 'mizellenwasser.jpg', 2),
(16, 'Pflegecreme Gesicht & Körper', 125, 'ml', 8.9, 'Spendet Feuchtigkeit für Gesicht und Körper', 'pflegecreme.jpg', 4),
(17, '3 in 1 Marine Make-up Entferner Jelly', 150, 'ml', 13.9, 'Entfernt Make-up, reinigt das Gesicht und die Augen in nur einem Schritt. Die ultra-frische Jellytextur lässt die Haut atmen', 'marine-makeup-entferner.jpg', 2),
(18, 'Duschbad Hammam Arganöl-Rosenwasser', 200, 'ml', 2.99, 'Inspiriert von den Schönheits-Geheimnissen des Orients', 'duschbad-hammam.jpg', 4),
(19, 'Pflanzliches Körperpeeling', 150, 'ml', 5.99, 'Seidig-glatte Haut, verführerisch schön!', 'pflanzliches-korperpeeling.jpg', 4),
(20, 'Handcreme Kokosnuss', 30, 'ml', 2.5, 'Feuchtigkeitsspendend mit einem sinnlichen Duft', 'handcreme-kokosnuss.jpg', 4),
(21, 'Shampoo Silberglanz-Reflexe', 200, 'ml', 4.99, 'Silberne Highlights für platinblondes, weißes oder graues Haar', 'shampoo-silberglanz.jpg', 5),
(22, 'Dusch-Shampoo Konzentrat Monoï', 100, 'ml', 2.95, 'Konzentriertes Dusch-Shampoo mit verführerischem Monoï-Duft', 'shampoo-dusch-monoi.jpg', 5),
(23, 'Nutrition Maske - Längen', 35, 'ml', 2.99, 'Pflegt die Längen intensiv und macht sie seidig-weich', 'nutrition-maske.jpg', 5),
(24, 'Repair Maske - Spitzen', 35, 'ml', 2.99, 'Repariert bis in die Tiefe und stärkt die Spitzen', 'repair-maske.jpg', 5),
(25, 'Festes Shampoo Nutrition', 60, 'g', 6.99, 'Weniger Plastik, Mehr Beauty', 'festes-nutrition.jpg', 5),
(26, 'Festes Shampoo mild & sanft', 60, 'g', 6.99, 'Weniger Plastik, Mehr Beauty', 'mild-sanft-festes.jpg', 5),
(27, 'Supra Essence Korrigierendes Serum', 50, 'ml', 59, 'Für eine intensiv erneuerte Haut', 'supra-essence-serum.jpg', 1),
(28, 'Duschgel Kirschblüten', 200, 'ml', 4.75, 'Der zarte Duft der Kirschblüte', 'duschgel-kirschbluten.jpg', 3),
(29, 'Pflegender Lipbalm getönt Himbeere', 5, 'g', 2.5, 'Für natürlich schöne und gepflegte Lippen', 'himbeere-lipbalm.jpg', 2),
(30, 'Duftspray Pfirsich-Sternanis', 100, 'ml', 10.9, 'Zum Aufsprühen auf Körper und Haare.', 'duftspray-pfirsich-sternanis.jpg', 3),
(31, 'Comme une Evidence - Parfümierte Körpermilch', 200, 'ml', 9.45, 'Die Intensität eines Augenblicks!', 'comme-u-evidence.jpg', 1),
(32, 'Eau de Parfum Garden Party 30ml', 30, 'ml', 19.95, 'Prickelnde Frühlingsfrische', 'garden-party.jpg', 3),
(33, 'Eau de Parfum Nouveau Genre - 100ml', 100, 'ml', 30, 'Das Elixir der Kraft', 'nouveau.jpg', 3),
(34, 'Eau de Parfum Matin Blanc', 30, 'ml', 19.95, 'Ein sonniger Start in den Tag', 'matinblanc.jpg', 3),
(35, 'Duschbad Magnolie-Weißer Tee', 200, 'ml', 2.99, 'Ein zart duftendes Duschbad mit entspannender Wirkung', 'dusch-magnolie.jpg', 4),
(36, 'Duschbad Kaffeebohne', 400, 'ml', 3.5, 'Dusch-Genuss pur: ein mildes Duschcreme mit sinnlichem Duft', 'kaffeebohne.jpg', 4),
(37, 'Trockenes Öl multi-use', 125, 'ml', 7.99, 'Das Multitalent für Körper & Haare', 'trockenesol.jpg', 4),
(38, 'Gel-Creme Non-Stop Feuchtigkeit 48H', 50, 'ml', 15.9, 'Für eine Non-Stop mit Feuchtigkeit versorgte Haut', 'feuchtigkeit48h.jpg', 1),
(39, 'Intensivpflege Tag & Nacht', 75, 'ml', 39.9, 'Versorgt die Haut intensiv mit Aufbaustoffen', 'tag-nacht.jpg', 1),
(40, 'Mattierende Gel-Creme', 50, 'ml', 15.9, 'Wirkt dauerhaft ausgleichend und verkleinert die Poren', 'mattierende-creme.jpg', 1),
(41, 'Glättende Tagespflege Glow', 50, 'ml', 25.9, 'Die Haut wird sofort mit Feuchtigkeit versorgt und erhält ihre natürliche Ausstrahlung zurück.', 'glattende-glow.jpg', 1),
(42, 'Regenerierende Creme Tag & Nacht', 50, 'ml', 12.9, 'Regeneriert die Haut und pflegt sie geschmeidig zart', 'regenerierende-creme.jpg', 1),
(43, 'Antifalten Verwöhn-Augenpflege', 14, 'ml', 21.9, 'Intensiv pflegende Augencreme', 'antifalten-augenpflege.jpg', 1);

-- --------------------------------------------------------

--
-- Table structure for table `shippingcost`
--

CREATE TABLE `shippingcost` (
  `id` int(11) NOT NULL,
  `cost` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `shippingcost`
--

INSERT INTO `shippingcost` (`id`, `cost`) VALUES
(1, 3.99),
(2, 0);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `cartitems`
--
ALTER TABLE `cartitems`
  ADD PRIMARY KEY (`id`),
  ADD KEY `ProductId` (`ProductId`);

--
-- Indexes for table `category`
--
ALTER TABLE `category`
  ADD PRIMARY KEY (`Id`);

--
-- Indexes for table `customers`
--
ALTER TABLE `customers`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `ordereditems`
--
ALTER TABLE `ordereditems`
  ADD KEY `orderID` (`orderID`),
  ADD KEY `ProductId` (`ProductId`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`id`),
  ADD KEY `shippingCostId` (`shippingCostId`),
  ADD KEY `customerId` (`customerId`);

--
-- Indexes for table `products`
--
ALTER TABLE `products`
  ADD PRIMARY KEY (`id`),
  ADD KEY `Img` (`Img`),
  ADD KEY `catId` (`catId`);

--
-- Indexes for table `shippingcost`
--
ALTER TABLE `shippingcost`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `cartitems`
--
ALTER TABLE `cartitems`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;

--
-- AUTO_INCREMENT for table `customers`
--
ALTER TABLE `customers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;

--
-- AUTO_INCREMENT for table `orders`
--
ALTER TABLE `orders`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=21;

--
-- AUTO_INCREMENT for table `products`
--
ALTER TABLE `products`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=44;

--
-- AUTO_INCREMENT for table `shippingcost`
--
ALTER TABLE `shippingcost`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `cartitems`
--
ALTER TABLE `cartitems`
  ADD CONSTRAINT `cartitems_ibfk_1` FOREIGN KEY (`ProductId`) REFERENCES `products` (`id`);

--
-- Constraints for table `ordereditems`
--
ALTER TABLE `ordereditems`
  ADD CONSTRAINT `ordereditems_ibfk_1` FOREIGN KEY (`orderID`) REFERENCES `orders` (`id`),
  ADD CONSTRAINT `ordereditems_ibfk_2` FOREIGN KEY (`ProductId`) REFERENCES `products` (`id`);

--
-- Constraints for table `orders`
--
ALTER TABLE `orders`
  ADD CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`customerId`) REFERENCES `customers` (`id`),
  ADD CONSTRAINT `orders_ibfk_2` FOREIGN KEY (`shippingCostId`) REFERENCES `shippingcost` (`id`),
  ADD CONSTRAINT `orders_ibfk_3` FOREIGN KEY (`customerId`) REFERENCES `customers` (`id`);

--
-- Constraints for table `products`
--
ALTER TABLE `products`
  ADD CONSTRAINT `products_ibfk_1` FOREIGN KEY (`catId`) REFERENCES `category` (`Id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
