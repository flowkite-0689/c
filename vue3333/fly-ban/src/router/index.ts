import { createRouter, createWebHistory } from 'vue-router'
import HomePage from '../views/HomePage.vue'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomePage
    },
    {
      path: '/staff',
      name: 'staff',
      component: () => import('../views/StaffView.vue')
    },
    {
      path: '/passenger',
      name: 'passenger',
      component: () => import('../views/PassengerView.vue')
    }
  ]
})

export default router
