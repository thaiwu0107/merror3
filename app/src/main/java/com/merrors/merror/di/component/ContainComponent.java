package com.merrors.merror.di.component;

import com.jess.arms.di.scope.ActivityScope;

import dagger.Component;

import com.jess.arms.di.component.AppComponent;

import com.merrors.merror.di.module.ContainModule;

import com.merrors.merror.mvp.ui.activity.ContainActivity;

@ActivityScope
@Component(modules = ContainModule.class, dependencies = AppComponent.class)
public interface ContainComponent {
    void inject(ContainActivity activity);
}