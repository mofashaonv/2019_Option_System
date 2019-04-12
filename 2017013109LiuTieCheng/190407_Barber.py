#!/usr/bin/env python3
# -*- coding: utf-8 -*-

class BarberCustomer:
  waitPerson = 0
  mutex = 1
  seatNum = 15    # 空座椅数量

  barberAvailable = 0
  customerAvailable = 0

  def Barber():
    while true:
      wait(customerAvailable)
      wait(mutex)
      seatNum += 1
      signal(barberAvailable)
      signal(mutex)

  def Customer():
    while true:
      wait(mutex)
      if (seatNum > 0):
        seatNum -= 1
        signal(customerAvailable)
        signal(mutex)
        wait(barberAvailable)
      else:
        signal(mutex)

def main():
  while true:
    BarberCustomer.Barber()
    BarberCustomer.Customer()

if __name__ == '__main__':
  main()