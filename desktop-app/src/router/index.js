import Vue from 'vue'
import Router from 'vue-router'
import AppLayout from '../components/admin/AppLayout'
import AuthLayout from '../components/auth/AuthLayout'
import lazyLoading from './lazyLoading'

Vue.use(Router)

const demoRoutes = []
if (process.env.NODE_ENV === 'development') {
  const VueBook = require('vue-book').default

  demoRoutes.push(
    VueBook(require.context('./..', true, /.demo.vue$/), '/demo'),
    VueBook(require.context('./../components', true, /.vue$/), '/presentation'),
  )
}

const EmptyParentComponent = {
  template: '<router-view></router-view>',
}

export default new Router({
  routes: [
    ...demoRoutes,
    {
      path: '*',
      redirect: { name: 'home' },
    },
    {
      path: '/auth',
      component: AuthLayout,
      children: [
        {
          name: 'login',
          path: 'login',
          component: lazyLoading('auth/login/Login'),
        },
        {
          name: 'signup',
          path: 'signup',
          component: lazyLoading('auth/signup/Signup'),
        },
        {
          path: '',
          redirect: { name: 'login' },
        },
      ],
    },
    {
      path: '/404',
      component: EmptyParentComponent,
      children: [
        {
          name: 'not-found-advanced',
          path: 'not-found-advanced',
          component: lazyLoading('pages/404-pages/VuesticPageNotFoundSearch')
        },
        {
          name: 'not-found-simple',
          path: 'not-found-simple',
          component: lazyLoading('pages/404-pages/VuesticPageNotFoundSimple')
        },
        {
          name: 'not-found-custom',
          path: 'not-found-custom',
          component: lazyLoading('pages/404-pages/VuesticPageNotFoundCustom')
        },
        {
          name: 'not-found-large-text',
          path: '/pages/not-found-large-text',
          component: lazyLoading('pages/404-pages/VuesticPageNotFoundLargeText')
        }
      ]
    },
    {
      name: 'Admin',
      path: '/admin',
      component: AppLayout,
      children: [
        {
          name: 'dashboard',
          path: 'dashboard',
          component: lazyLoading('dashboard/Dashboard')
        },
        {
          name: 'home',
          path: 'home',
          component: lazyLoading('cansat/home'),
          default: true,
        },
        {
          name: 'project',
          path: 'project',
          component: EmptyParentComponent,
          children: [
            {
              name: 'newProject',
              path: 'newProject',
              component: lazyLoading('cansat/project/newProject'),
            },
            {
              name: 'saveProject',
              path: 'saveProject',
              component: lazyLoading('cansat/project/saveProject'),
            },            
            {
              name: 'configProject',
              path: 'configProject',
              component: lazyLoading('cansat/project/configProject')
            }
          ],
        },
        {
          name: 'mission',
          path: 'mission',
          component: EmptyParentComponent,
          children: [
            {
              name: 'newMission',
              path: 'newMission',
              component: lazyLoading('cansat/mission/newMission'),
            },
            {
              name: 'openMission',
              path: 'openMission',
              component: lazyLoading('cansat/mission/openMission')
            },
            {
              name: 'configMission',
              path: 'configMission',
              component: lazyLoading('cansat/mission/configMission')
            },
            {
              name: 'exporDataMission',
              path: 'exporDataMission',
              component: lazyLoading('cansat/mission/exporDataMission')
            }
          ],
        },
        {
          name: 'sat',
          path: 'sat',
          component: EmptyParentComponent,
          children: [
            {
              name: 'linkSat',
              path: 'linkSat',
              component: lazyLoading('cansat/sat/linkSat'),
            },
            {
              name: 'testSat',
              path: 'testSat',
              component: lazyLoading('cansat/sat/testSat'),
            },
            {
              name: 'resourcesSat',
              path: 'resourcesSat',
              component: lazyLoading('cansat/sat/resourcesSat'),
            },
            {
              name: 'newSensor',
              path: 'newSensor',
              component: lazyLoading('cansat/sat/newSensor'),
            },

          ],
          
        },
        {
          name: 'notificationsCenter',
          path: 'notificationsCenter',
          component: lazyLoading('cansat/notifications/notificationsCenter'),
        }
      ],
    },
  ],
})
