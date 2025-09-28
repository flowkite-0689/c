/**
 * 航班信息接口定义
 * 对应 C 程序中的 Plane 结构体
 */
export interface Plane {
  ID: string;
  Start_Place: string;
  End_Place: string;
  price: number;
  Buy_num: number;
  Max_num: number;
  time: string;
}

/**
 * 乘客信息接口定义
 * 对应 C 程序中的 Person 结构体
 */
export interface Person {
  AirPlane_Num: number;
  person_name: string;
}

/**
 * 用户角色枚举
 */
export enum UserRole {
  PASSENGER = 'passenger',
  STAFF = 'staff'
}
