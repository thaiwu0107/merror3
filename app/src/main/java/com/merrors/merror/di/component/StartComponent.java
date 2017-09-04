package com.merrors.merror.di.component;

import com.jess.arms.di.scope.ActivityScope;

import dagger.Component;

import com.jess.arms.di.component.AppComponent;

import com.merrors.merror.di.module.StartModule;

import com.merrors.merror.mvp.ui.activity.StartActivity;

@ActivityScope
@Component(modules = StartModule.class, dependencies = AppComponent.class)
public interface StartComponent {
    void inject(StartActivity activity);
}