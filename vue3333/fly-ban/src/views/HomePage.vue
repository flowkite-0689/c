<script setup lang="ts">
import { ref } from 'vue'
import { UserRole } from '../types'
import { useRouter } from 'vue-router'

const router = useRouter()
const userRole = ref<UserRole | null>(null)

// 选择用户角色
const selectRole = (role: UserRole) => {
  userRole.value = role
  if (role === UserRole.STAFF) {
    router.push('/staff')
  } else {
    router.push('/passenger')
  }
}
</script>

<template>
  <div class="home-container">
    <div class="welcome-section">
      <h1>欢迎进入飞机票务系统</h1>
      <p>请选择您的身份</p>

      <div class="role-selection">
        <button
          class="role-button staff"
          @click="selectRole(UserRole.STAFF)"
        >
          票务人员
        </button>
        <button
          class="role-button passenger"
          @click="selectRole(UserRole.PASSENGER)"
        >
          旅客
        </button>
      </div>
    </div>
  </div>
</template>

<style scoped>
.home-container {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  min-height: 100vh;
  padding: 20px;
  background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
}

.welcome-section {
  background-color: white;
  border-radius: 10px;
  box-shadow: 0 8px 20px rgba(0, 0, 0, 0.1);
  padding: 40px;
  text-align: center;
  max-width: 500px;
  width: 100%;
}

h1 {
  color: #2c3e50;
  margin-bottom: 10px;
}

p {
  color: #7f8c8d;
  margin-bottom: 30px;
}

.role-selection {
  display: flex;
  justify-content: space-around;
  margin-top: 20px;
}

.role-button {
  padding: 15px 30px;
  border: none;
  border-radius: 5px;
  font-size: 16px;
  font-weight: bold;
  cursor: pointer;
  transition: all 0.3s ease;
}

.role-button.staff {
  background-color: #3498db;
  color: white;
}

.role-button.staff:hover {
  background-color: #2980b9;
}

.role-button.passenger {
  background-color: #2ecc71;
  color: white;
}

.role-button.passenger:hover {
  background-color: #27ae60;
}
</style>
