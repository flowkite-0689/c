<script setup lang="ts">
import { ref, computed } from 'vue'
import { flightStore } from '../stores/flightStore'
import { useRouter } from 'vue-router'

const router = useRouter()
const activeTab = ref(0)
const personName = ref('')
const selectedFlightIndex = ref(-1)
const errorMessage = ref('')
const successMessage = ref('')
const searchName = ref('')
const ticketResults = ref<any[]>([])
const hasSearched = ref(false)

// 切换到主页
const goToHome = () => {
  router.push('/')
}

// 预订航班
const bookFlight = () => {
  if (!personName.value.trim()) {
    errorMessage.value = '请输入姓名'
    return
  }

  if (selectedFlightIndex.value < 0) {
    errorMessage.value = '请选择航班'
    return
  }

  const result = flightStore.bookFlight(selectedFlightIndex.value, personName.value)

  if (result.success) {
    successMessage.value = result.message
    errorMessage.value = ''

    // 重置表单
    personName.value = ''
    selectedFlightIndex.value = -1

    // 3秒后清除成功消息
    setTimeout(() => {
      successMessage.value = ''
    }, 3000)
  } else {
    errorMessage.value = result.message
  }
}

// 查询个人订票信息
const searchTickets = () => {
  if (!searchName.value.trim()) {
    errorMessage.value = '请输入姓名'
    return
  }

  ticketResults.value = flightStore.getPersonTickets(searchName.value)
  hasSearched.value = true

  if (ticketResults.value.length === 0) {
    errorMessage.value = '未找到该乘客订票信息'
  } else {
    errorMessage.value = ''
  }
}

// 改签
const changeTicket = (ticketIndex: number, newFlightIndex: number) => {
  if (newFlightIndex < 0) {
    errorMessage.value = '请选择新航班'
    return
  }

  const result = flightStore.changeTicket(ticketIndex, newFlightIndex)

  if (result.success) {
    successMessage.value = result.message
    errorMessage.value = ''

    // 更新查询结果
    searchTickets()

    // 3秒后清除成功消息
    setTimeout(() => {
      successMessage.value = ''
    }, 3000)
  } else {
    errorMessage.value = result.message
  }
}

// 退票
const returnTicket = (ticketIndex: number) => {
  if (confirm('确定要退票吗？')) {
    const result = flightStore.returnTicket(ticketIndex)

    if (result.success) {
      successMessage.value = result.message
      errorMessage.value = ''

      // 更新查询结果
      searchTickets()

      // 3秒后清除成功消息
      setTimeout(() => {
        successMessage.value = ''
      }, 3000)
    } else {
      errorMessage.value = result.message
    }
  }
}

// 计算可用航班（未满员）
const availableFlights = computed(() => {
  return flightStore.planes.filter(plane => plane.Buy_num < plane.Max_num)
})
</script>

<template>
  <div class="passenger-container">
    <div class="header">
      <h1>飞机票务系统 - 乘客界面</h1>
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
        订票
      </button>
      <button
        :class="['tab-button', { active: activeTab === 1 }]"
        @click="activeTab = 1"
      >
        改签
      </button>
      <button
        :class="['tab-button', { active: activeTab === 2 }]"
        @click="activeTab = 2"
      >
        退票
      </button>
      <button
        :class="['tab-button', { active: activeTab === 3 }]"
        @click="activeTab = 3"
      >
        浏览航班信息
      </button>
      <button
        :class="['tab-button', { active: activeTab === 4 }]"
        @click="activeTab = 4"
      >
        查询个人订票信息
      </button>
    </div>

    <div class="tab-content">
      <!-- 订票 -->
      <div v-if="activeTab === 0" class="tab-pane">
        <h2>订票</h2>
        <form @submit.prevent="bookFlight" class="booking-form">
          <div class="form-group">
            <label for="personName">姓名:</label>
            <input
              id="personName"
              v-model="personName"
              type="text"
              required
            />
          </div>

          <div class="form-group">
            <label for="flightSelect">选择航班:</label>
            <select
              id="flightSelect"
              v-model="selectedFlightIndex"
              required
            >
              <option value="-1" disabled>请选择航班</option>
              <option
                v-for="(plane, index) in flightStore.planes"
                :key="index"
                :value="index"
                :disabled="plane.Buy_num >= plane.Max_num"
              >
                航班 {{ index }}: {{ plane.ID }} - {{ plane.Start_Place }} → {{ plane.End_Place }}
                ({{ plane.time }}) - ¥{{ plane.price.toFixed(2) }}
                [剩余: {{ plane.Max_num - plane.Buy_num }}]
              </option>
            </select>
          </div>

          <button type="submit" class="submit-button">预订</button>
        </form>

        <div class="flights-section">
          <h3>可用航班信息</h3>
          <div v-if="flightStore.planes.length === 0" class="empty-message">
            暂无航班信息
          </div>
          <div v-else class="flights-list">
            <div
              v-for="(plane, index) in flightStore.planes"
              :key="index"
              :class="['flight-item', { 'flight-full': plane.Buy_num >= plane.Max_num }]"
              @click="selectedFlightIndex = index"
            >
              <div class="flight-info">
                <h3>航班编号: {{ index }}</h3>
                <p>ID: {{ plane.ID }}</p>
                <p>始发地: {{ plane.Start_Place }} → 目的地: {{ plane.End_Place }}</p>
                <p>起飞时间: {{ plane.time }}</p>
                <p>价格: ¥{{ plane.price.toFixed(2) }}</p>
                <p>剩余机票: {{ plane.Max_num - plane.Buy_num }}</p>
                <div v-if="plane.Buy_num >= plane.Max_num" class="sold-out-badge">已售罄</div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- 改签 -->
      <div v-else-if="activeTab === 1" class="tab-pane">
        <h2>改签</h2>
        <div class="search-section">
          <div class="form-group">
            <label for="searchName">请输入姓名:</label>
            <input
              id="searchName"
              v-model="searchName"
              type="text"
              placeholder="输入姓名查询订票信息"
            />
            <button @click="searchTickets" class="search-button">查询</button>
          </div>
        </div>

        <div v-if="hasSearched && ticketResults.length === 0" class="empty-message">
          未找到该乘客订票信息
        </div>

        <div v-if="ticketResults.length > 0" class="tickets-list">
          <div v-for="result in ticketResults" :key="result.personIndex" class="ticket-item">
            <div class="ticket-info">
              <h3>订单编号: {{ result.personIndex }}</h3>
              <p>姓名: {{ result.person.person_name }}</p>
              <p>航班编号: {{ result.person.AirPlane_Num }}</p>
              <p>航班ID: {{ result.flight.ID }}</p>
              <p>始发地: {{ result.flight.Start_Place }} → 目的地: {{ result.flight.End_Place }}</p>
              <p>起飞时间: {{ result.flight.time }}</p>
              <p>价格: ¥{{ result.flight.price.toFixed(2) }}</p>
            </div>

            <div class="change-ticket-form">
              <h4>改签到新航班:</h4>
              <select v-model="result.newFlightIndex">
                <option value="-1" disabled>请选择新航班</option>
                <option
                  v-for="(plane, index) in availableFlights"
                  :key="index"
                  :value="index"
                  :disabled="index === result.person.AirPlane_Num"
                >
                  航班 {{ index }}: {{ plane.ID }} - {{ plane.Start_Place }} → {{ plane.End_Place }}
                  ({{ plane.time }}) - ¥{{ plane.price.toFixed(2) }}
                </option>
              </select>
              <button
                @click="changeTicket(result.personIndex, result.newFlightIndex)"
                class="change-button"
                :disabled="!result.newFlightIndex || result.newFlightIndex < 0"
              >
                确认改签
              </button>
            </div>
          </div>
        </div>
      </div>

      <!-- 退票 -->
      <div v-else-if="activeTab === 2" class="tab-pane">
        <h2>退票</h2>
        <div class="search-section">
          <div class="form-group">
            <label for="searchNameReturn">请输入姓名:</label>
            <input
              id="searchNameReturn"
              v-model="searchName"
              type="text"
              placeholder="输入姓名查询订票信息"
            />
            <button @click="searchTickets" class="search-button">查询</button>
          </div>
        </div>

        <div v-if="hasSearched && ticketResults.length === 0" class="empty-message">
          未找到该乘客订票信息
        </div>

        <div v-if="ticketResults.length > 0" class="tickets-list">
          <div v-for="result in ticketResults" :key="result.personIndex" class="ticket-item">
            <div class="ticket-info">
              <h3>订单编号: {{ result.personIndex }}</h3>
              <p>姓名: {{ result.person.person_name }}</p>
              <p>航班编号: {{ result.person.AirPlane_Num }}</p>
              <p>航班ID: {{ result.flight.ID }}</p>
              <p>始发地: {{ result.flight.Start_Place }} → 目的地: {{ result.flight.End_Place }}</p>
              <p>起飞时间: {{ result.flight.time }}</p>
              <p>价格: ¥{{ result.flight.price.toFixed(2) }}</p>
            </div>

            <button @click="returnTicket(result.personIndex)" class="return-button">
              退票
            </button>
          </div>
        </div>
      </div>

      <!-- 浏览航班信息 -->
      <div v-else-if="activeTab === 3" class="tab-pane">
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

      <!-- 查询个人订票信息 -->
      <div v-else-if="activeTab === 4" class="tab-pane">
        <h2>查询个人订票信息</h2>
        <div class="search-section">
          <div class="form-group">
            <label for="searchNameInfo">请输入姓名:</label>
            <input
              id="searchNameInfo"
              v-model="searchName"
              type="text"
              placeholder="输入姓名查询订票信息"
            />
            <button @click="searchTickets" class="search-button">查询</button>
          </div>
        </div>

        <div v-if="hasSearched && ticketResults.length === 0" class="empty-message">
          未找到该乘客订票信息
        </div>

        <div v-if="ticketResults.length > 0" class="tickets-list">
          <div v-for="result in ticketResults" :key="result.personIndex" class="ticket-item">
            <div class="ticket-info">
              <h3>订单编号: {{ result.personIndex }}</h3>
              <p>姓名: {{ result.person.person_name }}</p>
              <p>航班编号: {{ result.person.AirPlane_Num }}</p>
              <p>航班ID: {{ result.flight.ID }}</p>
              <p>始发地: {{ result.flight.Start_Place }} → 目的地: {{ result.flight.End_Place }}</p>
              <p>起飞时间: {{ result.flight.time }}</p>
              <p>价格: ¥{{ result.flight.price.toFixed(2) }}</p>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>
.passenger-container {
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

.booking-form {
  margin-bottom: 30px;
}

.form-group {
  margin-bottom: 15px;
}

.form-group label {
  display: block;
  margin-bottom: 5px;
  font-weight: bold;
}

.form-group input,
.form-group select {
  width: 100%;
  padding: 10px;
  border: 1px solid #ddd;
  border-radius: 5px;
}

.submit-button,
.search-button {
  padding: 12px;
  background-color: #28a745;
  color: white;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-size: 16px;
  font-weight: bold;
}

.submit-button:hover,
.search-button:hover {
  background-color: #218838;
}

.search-button {
  margin-top: 10px;
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
  cursor: pointer;
  transition: all 0.3s ease;
}

.flight-item:hover {
  box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
  transform: translateY(-2px);
}

.flight-item.flight-full {
  opacity: 0.7;
  background-color: #f8f9fa;
  cursor: not-allowed;
}

.flight-info h3 {
  margin-top: 0;
  color: #007bff;
}

.sold-out-badge {
  position: absolute;
  top: 10px;
  right: 10px;
  background-color: #dc3545;
  color: white;
  padding: 5px 10px;
  border-radius: 3px;
  font-size: 12px;
  font-weight: bold;
}

.empty-message {
  text-align: center;
  padding: 20px;
  color: #6c757d;
  font-style: italic;
}

.search-section {
  margin-bottom: 20px;
}

.tickets-list {
  display: grid;
  grid-template-columns: 1fr;
  gap: 20px;
}

.ticket-item {
  border: 1px solid #ddd;
  border-radius: 5px;
  padding: 15px;
  background-color: #f8f9fa;
}

.ticket-info h3 {
  margin-top: 0;
  color: #007bff;
}

.change-ticket-form {
  margin-top: 15px;
  padding-top: 15px;
  border-top: 1px solid #ddd;
}

.change-ticket-form select {
  width: 100%;
  padding: 10px;
  margin-bottom: 10px;
  border: 1px solid #ddd;
  border-radius: 5px;
}

.change-button,
.return-button {
  padding: 8px 15px;
  border: none;
  border-radius: 5px;
  cursor: pointer;
  font-weight: bold;
}

.change-button {
  background-color: #17a2b8;
  color: white;
}

.change-button:hover {
  background-color: #138496;
}

.change-button:disabled {
  background-color: #6c757d;
  cursor: not-allowed;
}

.return-button {
  background-color: #dc3545;
  color: white;
  margin-top: 15px;
}

.return-button:hover {
  background-color: #c82333;
}

@keyframes fadeIn {
  from { opacity: 0; }
  to { opacity: 1; }
}

@media (max-width: 768px) {
  .flights-list {
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
