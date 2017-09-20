# Proposal for RTFS - Real-Time FileSystem
Authors: Domenik Irrgang, Tobias Derksen <br>
Date: Venlo, 20th September 2017

## Introduction
This project will be carried out in context of the module IOT-CPP of the Informatics study at Fontys School of Technology Venlo (NL).

The purpose of this project is to meet the learning goals of the module which includes low-level C programming, memory management and real-time computations. All of these topics are included in this project.

## Overview
The goal of this project is to write a file system which can be loaded into a Linux kernel as a kernel module. Some examples of existing file systems are btrfs, ext4 or xfs.
Nevertheless, this project is not meant to ever produce a file system which can be used in production. The file system is meant for educational purposes only.

### File System Structure
For the internal structure of the file system a Btree is going to get used to ensure a logarithmic read time. 

### Real-Time Component
To meet the real-time computation learning goal, the file system will support a special mount option which enables a memory write cache. This ensures a fast write on the file system while the real write will be done asynchronously at a later point in time. Application which needs a real time performance on this file system can instantly continue working while the file system writes down the data in background.

This is of-course a very dangerous option. If the power supply fails before all data has been written to disks, data will be lost and possibly the file system will be in an inconsistent state.
