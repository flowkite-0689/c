import { ref, reactive, computed } from 'vue'
import { type Plane, type Person } from '../types'

/**
 * 航班管理系统状态管理
 * 使用 Vue3 Composition API 实现
 */
export const useFlightStore = () => {
  // 常量定义，对应 C 程序中的宏定义
  const PERSON_MAXNUM = 100
  const PERSONNOTICKET_MAX = 10
  const MAX_SIZE = 10

  // 状态变量，对应 C 程序中的全局变量
  const size = ref(0)
  const person_size = ref(0)
  const personnoticket_size = ref(0)

  // 数据存储，对应 C 程序中的数组
  const planes = reactive<Plane[]>([])
  const persons = reactive<Person[]>([])
  const personsNoTicket = reactive<Person[]>([])

  /**
   * 添加航班信息
   * 对应 C 程序中的 InputAirPlane 函数
   */
  const addPlane = (plane: Plane): boolean => {
    if (size.value >= MAX_SIZE) {
      return false
    }

    planes.push({
      ...plane,
      Buy_num: Math.min(plane.Buy_num, plane.Max_num) // 确保已售票数不超过最大容量
    })

    size.value++
    return true
  }

  /**
   * 删除航班信息
   * 对应 C 程序中的 DelAirPlane 函数
   */
  const deletePlane = (index: number): boolean => {
    if (index < 0 || index >= size.value) {
      return false
    }

    planes.splice(index, 1)
    size.value--

    // 更新相关乘客信息
    updatePassengersAfterFlightDeletion(index)

    return true
  }

  /**
   * 更新航班删除后的乘客信息
   */
  const updatePassengersAfterFlightDeletion = (deletedIndex: number) => {
    // 移除已购买该航班的乘客
    for (let i = persons.length - 1; i >= 0; i--) {
      if (persons[i].AirPlane_Num === deletedIndex) {
        persons.splice(i, 1)
        person_size.value--
      } else if (persons[i].AirPlane_Num > deletedIndex) {
        // 更新大于删除航班编号的引用
        persons[i].AirPlane_Num--
      }
    }

    // 更新候补名单
    for (let i = personsNoTicket.length - 1; i >= 0; i--) {
      if (personsNoTicket[i].AirPlane_Num === deletedIndex) {
        personsNoTicket.splice(i, 1)
        personnoticket_size.value--
      } else if (personsNoTicket[i].AirPlane_Num > deletedIndex) {
        // 更新大于删除航班编号的引用
        personsNoTicket[i].AirPlane_Num--
      }
    }
  }

  /**
   * 预订航班
   * 对应 C 程序中的 BookAirPlane 函数
   */
  const bookFlight = (flightIndex: number, personName: string): { success: boolean; message: string } => {
    if (flightIndex < 0 || flightIndex >= size.value) {
      return { success: false, message: '航班编号无效' }
    }

    const flight = planes[flightIndex]

    // 检查航班是否已满
    if (flight.Buy_num >= flight.Max_num) {
      // 添加到候补名单
      if (personnoticket_size.value >= PERSONNOTICKET_MAX) {
        return { success: false, message: '候补名单已满' }
      }

      personsNoTicket.push({
        AirPlane_Num: flightIndex,
        person_name: personName
      })

      personnoticket_size.value++
      return { success: true, message: '航班已满，已添加到候补名单' }
    }

    // 正常预订
    if (person_size.value >= PERSON_MAXNUM) {
      return { success: false, message: '乘客数量已达上限' }
    }

    persons.push({
      AirPlane_Num: flightIndex,
      person_name: personName
    })

    person_size.value++
    flight.Buy_num++

    return { success: true, message: '预订成功' }
  }

  /**
   * 查询乘客订票信息
   * 对应 C 程序中的 CheckPerson 函数
   */
  const getPersonTickets = (personName: string) => {
    const tickets = persons
      .filter(p => p.person_name === personName)
      .map(p => {
        const flight = planes[p.AirPlane_Num]
        return {
          personIndex: persons.indexOf(p),
          person: p,
          flight
        }
      })

    return tickets
  }

  /**
   * 改签
   * 对应 C 程序中的 ChangeAirPlane 函数
   */
  const changeTicket = (ticketIndex: number, newFlightIndex: number): { success: boolean; message: string } => {
    if (ticketIndex < 0 || ticketIndex >= persons.length) {
      return { success: false, message: '订单编号无效' }
    }

    if (newFlightIndex < 0 || newFlightIndex >= size.value) {
      return { success: false, message: '航班编号无效' }
    }

    const oldFlightIndex = persons[ticketIndex].AirPlane_Num
    const newFlight = planes[newFlightIndex]

    // 检查新航班是否已满
    if (newFlight.Buy_num >= newFlight.Max_num) {
      return { success: false, message: '该航班已满员，无法改签' }
    }

    // 减少原航班的购票数
    planes[oldFlightIndex].Buy_num--

    // 更新乘客航班信息
    persons[ticketIndex].AirPlane_Num = newFlightIndex

    // 增加新航班的购票数
    newFlight.Buy_num++

    return { success: true, message: '改签成功' }
  }

  /**
   * 退票
   * 对应 C 程序中的 ReturnTicket 函数
   */
  const returnTicket = (ticketIndex: number): { success: boolean; message: string } => {
    if (ticketIndex < 0 || ticketIndex >= persons.length) {
      return { success: false, message: '订单编号无效' }
    }

    const flightIndex = persons[ticketIndex].AirPlane_Num

    // 减少航班的购票数
    planes[flightIndex].Buy_num--

    // 移除乘客信息
    persons.splice(ticketIndex, 1)
    person_size.value--

    // 检查候补名单，如果有人在等待，自动为其预订
    processWaitingList(flightIndex)

    return { success: true, message: '退票成功' }
  }

  /**
   * 处理候补名单
   */
  const processWaitingList = (flightIndex: number) => {
    // 查找候补名单中等待该航班的乘客
    const waitingIndex = personsNoTicket.findIndex(p => p.AirPlane_Num === flightIndex)

    if (waitingIndex >= 0) {
      const waitingPerson = personsNoTicket[waitingIndex]

      // 为候补乘客预订机票
      persons.push({
        AirPlane_Num: flightIndex,
        person_name: waitingPerson.person_name
      })

      person_size.value++
      planes[flightIndex].Buy_num++

      // 从候补名单中移除
      personsNoTicket.splice(waitingIndex, 1)
      personnoticket_size.value--
    }
  }

  /**
   * 获取航班的乘客列表
   * 对应 C 程序中的 PrintPerson 函数
   */
  const getFlightPassengers = (flightIndex: number) => {
    return persons
      .filter(p => p.AirPlane_Num === flightIndex)
      .map(p => p.person_name)
  }

  return {
    // 状态
    size,
    person_size,
    personnoticket_size,
    planes,
    persons,
    personsNoTicket,

    // 航班管理功能
    addPlane,
    deletePlane,

    // 乘客功能
    bookFlight,
    getPersonTickets,
    changeTicket,
    returnTicket,
    getFlightPassengers,

    // 常量
    PERSON_MAXNUM,
    PERSONNOTICKET_MAX,
    MAX_SIZE
  }
}

// 创建全局单例状态
export const flightStore = useFlightStore()
