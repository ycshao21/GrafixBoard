#pragma once

namespace _template_project_name_::cuda
{
void launch_vec_add(const float* a, const float* b, float* c, int n);
}  // namespace _template_project_name_::cuda

namespace _template_project_name_::cpu
{
void launch_vec_add(const float* a, const float* b, float* c, int n);
}  // namespace _template_project_name_::cpu
