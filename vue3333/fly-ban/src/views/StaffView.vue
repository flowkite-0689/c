<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { flightStore } from '../stores/flightStore'
import {type Plane } from '../types'
import { useRouter } from 'vue-router'

const router = useRouter()
const activeTab = ref(0)
const newPlane = ref<Plane>({
  ID: '',
  Start_Place: '',
  End_Place: '',
  price: 0,
  Buy_num: 0,
  Max_num: 0,
  time: ''
})
const errorMessage = ref('')
const successMessage = ref('')

// 切换到主页
const goToHome = () => {
  router.push('/')
}

// 添加航班
const addFlight = () => {
  // 验证输入
  if (!newPlane.value.ID || !newPlane.value.Start_Place || !newPlane.value.End_Place ||
      !newPlane.value.time || newPlane.value.price <= 0 || newPlane.value.Max_num <= 0) {
    errorMessage.value = '请填写所有必要信息'
    return
  }

  if (newPlane.value.Buy_num > newPlane.value.Max_num) {
    errorMessage.value = '已售出机票数量不能超过最大容量'
    return
  }

  // 添加航班
  const result = flightStore.addPlane({ ...newPlane.value })

  if (result) {
    successMessage.value = '添加航班成功'
    errorMessage.value = ''

    // 重置表单
    newPlane.value = {
      ID: '',
      Start_Place: '',
      End_Place: '',
      price: 0,
      Buy_num: 0,
      Max_num: 0,
      time: ''
    }

    // 3秒后清除成功消息
    setTimeout(() => {
      successMessage.value = ''
    }, 3000)
  } else {
    errorMessage.value = '添加航班失败，可能已达到最大航班数量'
  }
}

// 删除航班
const deleteFlight = (index: number) => {
  if (confirm(`确定要删除航班 ${flightStore.planes[index].ID} 吗？`)) {
    const result = flightStore.deletePlane(index)
    if (result) {
      successMessage.value = '删除航班成功'
      // 3秒后清除成功消息
      setTimeout(() => {
        successMessage.value = ''
      }, 3000)
    } else {
      errorMessage.value = '删除航班失败'
    }
  }
}

// 获取航班的乘客列表
const getPassengersForFlight = (index: number) => {
  return flightStore.getFlightPassengers(index)
}
</script>

<template>
  <div class="staff-container">
    <div class="header">
      <h1>飞机票务系统 - 管理员界面</h1>
      <button class="home-button" @click="goToHome">返回主页</button>
    </div>

    <div class="message-container">
      <div v-if="errorMessage" class="error-message">{{ errorMessage }}</div>
      <div v-if="successMessage" class="success-message">{{ successMessage }}</div>
    </div>

    <div class="tabs">
      <button
        :class="['tab-button', { active: activeTab === 0 }]"
        @click="activeTab = 0"
      >
        输入航班信息
      </button>
      <button
        :class="['tab-button', { active: activeTab === 1 }]"
        @click="activeTab = 1"
      >
        删除航班信息
      </button>
      <button
        :class="['tab-button', { active: activeTab === 2 }]"
        @click="activeTab = 2"
      >
        浏览航班信息
      </button>
      <button
        :class="['tab-button', { active: activeTab === 3 }]"
        @click="activeTab = 3"
      >
        浏览已订票信息
      </button>
      <button
        :class="['tab-button', { active: activeTab === 4 }]"
        @click="activeTab = 4"
      >
        查看候补名单
      </button>
    </div>

    <div class="tab-content">
      <!-- 输入航班信息 -->
      <div v-if="activeTab === 0" class="tab-pane">
        <h2>输入航班信息</h2>
        <form @submit.prevent="addFlight" class="flight-form">
          <div class="form-group">
            <label for="flightId">航班ID:</label>
            <input
              id="flightId"
              v-model="newPlane.ID"
              type="text"
              required
            />
          </div>

          <div class="form-group">
            <label for="startPlace">始发地:</label>
            <input
              id="startPlace"
              v-model="newPlane.Start_Place"
              type="text"
              required
            />
          </div>

          <div class="form-group">
            <label for="endPlace">目的地:</label>
            <input
              id="endPlace"
              v-model="newPlane.End_Place"
              type="text"
              required
            />
          </div>

          <div class="form-group">
            <label for="time">起飞时间:</label>
            <input
              id="time"
              v-model="newPlane.time"
              type="text"
              placeholder="YYYY-MM-DD HH:MM"
              required
            />
          </div>

          <div class="form-group">
            <label for="price">价格:</label>
            <input
              id="price"
              v-model.number="newPlane.price"
              type="number"
              min="0"
              step="0.01"
              required
            />
          </div>

          <div class="form-group">
            <label for="maxNum">最大乘坐人数:</label>
            <input
              id="maxNum"
              v-model.number="newPlane.Max_num"
              type="number"
              min="1"
              required
            />
          </div>

          <div class="form-group">
            <label for="buyNum">已售出机票数量:</label>
            <input
              id="buyNum"
              v-model.number="newPlane.Buy_num"
              type="number"
              min="0"
              :max="newPlane.Max_num"
              required
            />
          </div>

          <button type="submit" class="submit-button">添加航班</button>
        </form>
      </div>

      <!-- 删除航班信息 -->
      <div v-else-if="activeTab === 1" class="tab-pane">
        <h2>删除航班信息</h2>
        <div v-if="flightStore.planes.length === 0" class="empty-message">
          暂无航班信息
        </div>
        <div v-else class="flights-list">
          <div v-for="(plane, index) in flightStore.planes" :key="index" class="flight-item">
            <div class="flight-info">
              <h3>航班编号: {{ index }}</h3>
              <p>ID: {{ plane.ID }}</p>
              <p>始发地: {{ plane.Start_Place }} → 目的地: {{ plane.End_Place }}</p>
              <p>起飞时间: {{ plane.time }}</p>
              <p>价格: ¥{{ plane.price.toFixed(2) }}</p>
              <p>已售/总座位: {{ plane.Buy_num }}/{{ plane.Max_num }}</p>
            </div>
            <button @click="deleteFlight(index)" class="delete-button">删除</button>
          </div>
        </div>
      </div>

      <!-- 浏览航班信息 -->
      <div v-else-if="activeTab === 2" class="tab-pane">
        <h2>浏览航班信息</h2>
        <div v-if="flightStore.planes.length === 0" class="empty-message">
          暂无航班信息
        </div>
        <div v-else class="flights-list">
          <div v-for="(plane, index) in flightStore.planes" :key="index" class="flight-item">
            <div class="flight-info">
              <h3>航班编号: {{ index }}</h3>
              <p>ID: {{ plane.ID }}</p>
              <p>始发地: {{ plane.Start_Place }} → 目的地: {{ plane.End_Place }}</p>
              <p>起飞时间: {{ plane.time }}</p>
              <p>价格: ¥{{ plane.price.toFixed(2) }}</p>
              <p>已售/总座位: {{ plane.Buy_num }}/{{ plane.Max_num }}</p>
              <p>剩余机票: {{ plane.Max_num - plane.Buy_num }}</p>
            </div>
          </div>
        </div>
      </div>

      <!-- 浏览已订票信息 -->
      <div v-else-if="activeTab === 3" class="tab-pane">
        <h2>浏览已订票信息</h2>
        <div v-if="flightStore.planes.length === 0" class="empty-message">
          暂无航班信息
        </div>
        <div v-else class="flights-list">
          <div v-for="(plane, index) in flightStore.planes" :key="index" class="flight-item">
            <div class="flight-info">
              <h3>航班编号: {{ index }}</h3>
              <p>ID: {{ plane.ID }}</p>
              <p>始发地: {{ plane.Start_Place }} → 目的地: {{ plane.End_Place }}</p>
              <p>起飞时间: {{ plane.time }}</p>
              <p>价格: ¥{{ plane.price.toFixed(2) }}</p>
              <p>已售/总座位: {{ plane.Buy_num }}/{{ plane.Max_num }}</p>
              <p>剩余机票: {{ plane.Max_num - plane.Buy_num }}</p>

              <div class="passengers-section">
                <h4>乘客名单:</h4>
                <div v-if="getPassengersForFlight(index).length === 0" class="empty-message">
                  暂无乘客
                </div>
                <ul v-else class="passengers-list">
                  <li v-for="(passenger, pIndex) in getPassengersForFlight(index)" :key="pIndex">
                    {{ passenger }}
                  </li>
                </ul>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- 查看候补名单 -->
      <div v-else-if="activeTab === 4" class="tab-pane">
        <h2>查看候补名单</h2>
        <div v-if="flightStore.personsNoTicket.length === 0" class="empty-message">
          候补名单为空
        </div>
        <div v-else class="waiting-list">
          <div v-for="(person, index) in flightStore.personsNoTicket" :key="index" class="waiting-item">
            <h3>候补乘客 #{{ index + 1 }}</h3>
            <p>姓名: {{ person.person_name }}</p>
            <p>所需航班编号: {{ person.AirPlane_Num }}</p>
            <p v-if="flightStore.planes[person.AirPlane_Num]">
              航班ID: {{ flightStore.planes[person.AirPlane_Num].ID }}
            </p>
            <p v-if="flightStore.planes[person.AirPlane_Num]">
              始发地: {{ flightStore.planes[person.AirPlane_Num].Start_Place }}
            </p>
            <p v-if="flightStore.planes[person.AirPlane_Num]">
              目的地: {{ flightStore.planes[person.AirPlane_Num].End_Place }}
            </p>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.staff-container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 20px;
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
}

.home-button {
  padding: 10px 20px;
  background-color: #34495e;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-size: 14px;
}

.home-button:hover {
  background-color: #2c3e50;
}

.message-container {
  margin-bottom: 20px;
}

.error-message {
  background-color: #f8d7da;
  color: #721c24;
  padding: 10px;
  border-radius: 5px;
  margin-bottom: 10px;
}

.success-message {
  background-color: #d4edda;
  color: #155724;
  padding: 10px;
  border-radius: 5px;
  margin-bottom: 10px;
}

.tabs {
  display: flex;
  flex-wrap: wrap;
  margin-bottom: 20px;
  border-bottom: 1px solid #ddd;
}

.tab-button {
  padding: 10px 20px;
  background-color: #f8f9fa;
  border: none;
  cursor: pointer;
  font-size: 14px;
  transition: all 0.3s ease;
}

.tab-button:hover {
  background-color: #e9ecef;
}

.tab-button.active {
  background-color: #007bff;
  color: white;
}

.tab-content {
  background-color: white;
  border-radius: 5px;
  padding: 20px;
  box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
}

.tab-pane {
  animation: fadeIn 0.5s;
}

.flight-form {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
  gap: 20px;
}

.form-group {
  margin-bottom: 15px;
}

.form-group label {
  display: block;
  margin-bottom: 5px;
  font-weight: bold;
}

.form-group input {
  width: 100%;
  padding: 10px;
  border: 1px solid #ddd;
  border-radius: 5px;
}

.submit-button {
  grid-column: 1 / -1;
  padding: 12px;
  background-color: #28a745;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-size: 16px;
  font-weight: bold;
}

.submit-button:hover {
  background-color: #218838;
}

.flights-list {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(350px, 1fr));
  gap: 20px;
}

.flight-item {
  border: 1px solid #ddd;
  border-radius: 5px;
  padding: 15px;
  position: relative;
}

.flight-info h3 {
  margin-top: 0;
  color: #007bff;
}

.delete-button {
  position: absolute;
  top: 15px;
  right: 15px;
  padding: 5px 10px;
  background-color: #dc3545;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
}

.delete-button:hover {
  background-color: #c82333;
}

.empty-message {
  text-align: center;
  padding: 20px;
  color: #6c757d;
  font-style: italic;
}

.passengers-section {
  margin-top: 15px;
  border-top: 1px solid #eee;
  padding-top: 10px;
}

.passengers-list {
  padding-left: 20px;
}

.waiting-list {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(300px, 1fr));
  gap: 20px;
}

.waiting-item {
  border: 1px solid #ddd;
  border-radius: 5px;
  padding: 15px;
  background-color: #fff8dc;
}

.waiting-item h3 {
  margin-top: 0;
  color: #ff8c00;
}

@keyframes fadeIn {
  from { opacity: 0; }
  to { opacity: 1; }
}

@media (max-width: 768px) {
  .flight-form {
    grid-template-columns: 1fr;
  }

  .flights-list {
    grid-template-columns: 1fr;
  }

  .waiting-list {
    grid-template-columns: 1fr;
  }

  .tabs {
    flex-direction: column;
  }

  .tab-button {
    width: 100%;
    text-align: left;
    border-bottom: 1px solid #ddd;
  }
}
</style>
